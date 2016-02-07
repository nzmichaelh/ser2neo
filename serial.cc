// Blocking serial port.
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
#include "serial.h"

#include "board.h"

/// Initialise the hardware.
void Serial::init() {
    Chip_UART_Init(LPC_USART0);
    Chip_Clock_SetUARTClockDiv(ClockDiv);

    handle_ = LPC_UARTD_API->uart_setup(
        LPC_USART0_BASE, static_cast<uint8_t*>(static_cast<void*>(mem_)));

    UART_CONFIG_T cfg = {
        .sys_clk_in_hz = CONFIG_SYS_FREQ / ClockDiv,
        .baudrate_in_hz = 115200,
        .config = 1,
        .sync_mod = 0,
        .error_en = NO_ERR_EN,
    };

    auto frg = LPC_UARTD_API->uart_init(handle_, &cfg);
    Chip_SYSCTL_SetUSARTFRGDivider(0xFF);
    Chip_SYSCTL_SetUSARTFRGMultiplier(frg);
}

/// Wait until the hardware is free then start the transmit.
void Serial::putch(uint8_t ch) {
    LPC_UARTD_API->uart_put_char(handle_, ch);
}

/// Send a string.
void Serial::putstr(const char* str) {
    for (; *str != '\0'; str++) {
        putch(*str);
    }
}

/// Wait until a character is received, then return it.
uint8_t Serial::getch() {
    return LPC_UARTD_API->uart_get_char(handle_);
}
