// Half-duplex interrupt based serial port.
//
#include "serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

Serial* Serial::instance_;

#define _clear(port, pin) port &= ~_BV(pin)
#define _set(port, pin) port |= _BV(pin)

// Initialise the hardware.
void Serial::init() {
    state_ = Idle;
    rx_full_ = false;

    instance_ = this;

    // Set up the GPIOs.
    _set(DDRB, TxPin);
    _set(PORTB, TxPin);
    _clear(DDRB, RxPin);

    // Set up the timer.
    static_assert(BitTime > 50, "BitTime is too short");
    static_assert(BitTime < 255*2/3, "BitTime may overflow");
    OCR1C = BitTime;
    TCCR1 = _BV(CTC1) | _BV(PWM1A);

    static_assert(Prescale == 2, "Mismatched prescaler");
    TCCR1 |= _BV(CS11);

    // Set up the pin change interrupt.
    PCMSK = _BV(RxPin);
    _set(GIMSK, PCIE);
}

// Handle the pin change interrupt by starting receive.
void Serial::pcint0() {
    _clear(PCMSK, RxPin);
    _clear(TIMSK, TOV1);  // PENDING

    OCR1C = (BitTime / 2 * 3) - (PCIntDelay / Prescale);
    TCNT1 = 0;
    _set(TIFR, TOV1);

    state_ = Receive;
    bits_ = 8;

    _set(TIMSK, TOV1);
}

// Handle the timer overflow interrupt by doing the next bit.
inline void Serial::timer1ovf() {
    switch (state_) {
        case Receive: {
            auto got = PINB;
            OCR1C = BitTime;
            if (bits_ > 0) {
                rxing_ >>= 1;
                if (got & _BV(RxPin)) {
                    rxing_ |= 0x80;
                }
                bits_--;
            } else {
                state_ = Idle;
                rxed_ = rxing_;
                rx_full_ = true;
            }
            break;
        }
        case Transmit:
            if (txing_ & 1) {
                _set(PORTB, TxPin);
            } else {
                _clear(PORTB, TxPin);
            }
            txing_ >>= 1;
            if (--bits_ == 0) {
                state_ = Idle;
            }
            break;
        default:
            break;
    }

    if (state_ == Idle) {
        _clear(TIMSK, TOV1);
        _set(PCMSK, RxPin);
    }
}

// Wait until the hardware is free then start the transmit.
void Serial::putch(uint8_t ch) {
    while (state_ != Idle) {
        sleep_cpu();
    }
    state_ = Transmit;
    txing_ = (ch << 1) | 0xFE00;
    bits_ = 10;

    TCNT1 = 0;
    _set(TIMSK, TOV1);
}

// Send a string.
void Serial::putstr(const char* str) {
    for (; *str != '\0'; str++) {
        putch(*str);
    }
}

// Wait until a character is received, then return it.
uint8_t Serial::getch() {
    while (!rx_full_) {
        sleep_cpu();
    }
    uint8_t ch = rxed_;
    rx_full_ = false;
    return ch;
}

inline void Serial::pcint0_bounce() { instance_->pcint0(); }

inline void Serial::timer1ovf_bounce() { instance_->timer1ovf(); }

ISR(TIMER1_OVF_vect) { Serial::timer1ovf_bounce(); }

ISR(PCINT0_vect) { Serial::pcint0_bounce(); }
