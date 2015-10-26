// Half-duplex interrupt based serial port.
//
#pragma once

#include <stdint.h>

// Half-duplex interrupt based serial port.
//
// Loosely based on AVR304.
//
class Serial {
   public:
    void init();

    void putch(uint8_t ch);
    void putstr(const char* str);
    
    uint8_t getch();

   private:
    enum State : uint8_t {
        Idle,
        Receive,
        Transmit,
    };

    // Note that these are all specific to an ATTINY85 USB board.
    static const int RxPin = 0;
    static const int TxPin = 2;

    static const auto Prescale = 2;
    static const auto Baud = 57600;
    static const auto BitTime = F_CPU / Prescale / Baud - 1;
    // Total number of clocks taken to service the pin change
    // interrupt and start the receive.  Use this to reduce the first
    // bit-and-a-half time so sampling occurs half way through the
    // first bit.
    static const uint8_t PCIntDelay = 140;

    void pcint0();
    void timer1ovf();

    volatile State state_;
    uint8_t rxing_;
    // Remaining number of bits to send or receive.
    uint8_t bits_;
    volatile uint8_t rxed_;
    volatile bool rx_full_;
    
    volatile uint16_t txing_;

    static Serial* instance_;

   public:
    // Interrupt handler entry points.
    static void pcint0_bounce();
    static void timer1ovf_bounce();
};
