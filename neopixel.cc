// NeoPixel ring driver.
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
#include "neopixel.h"
#include "chip.h"

#include <cstring>

#define CFG_ENABLE          (1<<0)
#define CFG_MASTER          (1<<2)

const uint16_t NeoPixel::bits_[] = {
    0b100100100100,
    0b100100100110,
    0b100100110100,
    0b100100110110,
    0b100110100100,
    0b100110100110,
    0b100110110100,
    0b100110110110,
    0b110100100100,
    0b110100100110,
    0b110100110100,
    0b110100110110,
    0b110110100100,
    0b110110100110,
    0b110110110100,
    0b110110110110,
};


/// Initialise the hardware.
void NeoPixel::init() {
    LPC_FMC->FLASHCFG = 0;            // 1 wait state instead of 1

    LPC_SWM->PINENABLE0 |= 1<<2;            // disable SWCLK
    LPC_SWM->PINASSIGN[4] = 0xFFFFFF03UL;     // SPI0_MOSI 3

    /* Enable SPI0 clock */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
    LPC_SYSCON->PRESETCTRL &= ~(1<<0);
    LPC_SYSCON->PRESETCTRL |= (1<<0);

    // 2.4 MHz, i.e. 12 MHz / 5
    LPC_SPI0->DIV = 4;
    LPC_SPI0->DLY = 0;

    LPC_SPI0->TXCTRL = SPI_TXDATCTL_FLEN(12-1) | SPI_TXDATCTL_RXIGNORE;

    LPC_SPI0->CFG = CFG_MASTER;
    LPC_SPI0->CFG |= CFG_ENABLE;
}

/// Set all the LEDs to off and restart at the first LED.
void NeoPixel::clear() {
    p_ = bytes_;
    memset(bytes_, 0, sizeof(bytes_));
}

/// Append a byte to the stream.
void NeoPixel::append(uint8_t b) {
    if (p_ <= (bytes_ + sizeof(bytes_))) {
        *p_++ = b;
    }
}

void NeoPixel::send(uint16_t cmd) {
    while ((LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0) {}
    LPC_SPI0->TXDAT = cmd;
}

/// Write to the LEDs.
void NeoPixel::write() {
    send(0);
    send(0);
    for (auto p = bytes_; p < p_; p++) {
        auto ch = *p;
        send(bits_[ch >> 4]);
        send(bits_[ch & 0xF]);
    }
}
