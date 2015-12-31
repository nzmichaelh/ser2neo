// Half-duplex interrupt based serial port.
//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once

#include <stdint.h>

/// Half-duplex interrupt based serial port.
///
/// Loosely based on AVR304.
///
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
