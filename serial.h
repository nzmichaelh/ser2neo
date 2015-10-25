#pragma once

#include <stdint.h>

class Serial {
   public:
    void init();

    void putch(uint8_t ch);
    uint8_t getch();

   private:
    enum State {
        Idle,
        Receive,
        Transmit,
    };

    static const int RxPin = 0;
    static const int TxPin = 2;
    static const int StatePin = 2;
    static const auto Prescale = 2;
    static const auto Baud = 57600;
    static const uint8_t BitTime = F_CPU / Prescale / Baud - 1;
    static const uint8_t PCIntDelay = 140;

    void pcint0();
    void timer1ovf();

    volatile State state_;
    uint8_t rxing_;
    volatile uint8_t rxed_;
    volatile bool rx_full_;
    volatile uint16_t txing_;
    uint8_t bits_;

    static Serial* instance_;

   public:
    static void pcint0_bounce();
    static void timer1ovf_bounce();
};
