#include "serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>

Serial* Serial::instance_;

#define _clear(port, pin) port &= ~_BV(pin)
#define _set(port, pin) port |= _BV(pin)

void Serial::init() {
    state_ = Idle;
    rx_full_ = false;

    instance_ = this;

    DDRB |= _BV(TxPin);
    DDRB &= ~_BV(RxPin);
    PORTB |= _BV(TxPin);

    OCR1C = BitTime;
    TCCR1 = _BV(CTC1) | _BV(PWM1A) | _BV(CS11);

    PCMSK = _BV(RxPin);
    _set(GIMSK, PCIE);
}

void Serial::pcint0() {
    _clear(PCMSK, RxPin);
    _clear(TIMSK, TOV1);  // PENDING

    OCR1C = (BitTime / 2 * 3) - (PCIntDelay / Prescale);
    TCNT1 = 0;
    _set(TIFR, TOV1);
    //    PORTB ^= _BV(StatePin);

    state_ = Receive;
    bits_ = 8;

    _set(TIMSK, TOV1);
}

inline void Serial::timer1ovf() {
    switch (state_) {
        case Receive: {
            auto got = PINB;
            //        PORTB ^= _BV(StatePin);
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
        //        _set(PORTB, StatePin);
    }
}

void Serial::putch(uint8_t ch) {
    while (state_ != Idle) {
    }
    state_ = Transmit;
    txing_ = (ch << 1) | 0xFE00;
    bits_ = 10;

    TCNT1 = 0;
    _set(TIMSK, TOV1);
}

uint8_t Serial::getch() {
    while (!rx_full_) {
    }
    uint8_t ch = rxed_;
    rx_full_ = false;
    return ch;
}

inline void Serial::pcint0_bounce() { instance_->pcint0(); }

inline void Serial::timer1ovf_bounce() { instance_->timer1ovf(); }

ISR(TIMER1_OVF_vect) { Serial::timer1ovf_bounce(); }

ISR(PCINT0_vect) { Serial::pcint0_bounce(); }

#if 0
uint8_t Serial::mark(uint8_t time) {
    uint8_t at = TCNT1 + time;
    OCR1A = at;
    TIFR |= _BV(OCF1A);
    return at;
}

void Serial::delay(uint8_t& at, uint8_t time) {
    at += time;
    while (!(TIFR & _BV(OCF1A))) {
    }
    OCR1A = at;
    TIFR |= _BV(OCF1A);
}

void Serial::putch(uint8_t ch) {
    PORTB &= ~_BV(TxPin);
    auto at = mark();
    delay(at);

    for (auto i = 0; i < 8; i++) {
        if (ch & 1) {
            PORTB |= _BV(TxPin);
        } else {
            PORTB &= ~_BV(TxPin);
        }
        ch >>= 1;
        delay(at);
    }
    PORTB |= _BV(TxPin);
    delay(at);
}

uint8_t Serial::getch() {
    uint8_t rx = 0;
    while (PINB & _BV(RxPin)) {
    }
    auto at = mark(BitTime/2*3);

    for (auto i = 0; i < 8; i++) {
        delay(at);
        rx >>= 1;
        if (PINB & _BV(RxPin)) {
            rx |= 0x80;
        }
    }
    delay(at, BitTime/4*3);
    return rx;
}
#endif
