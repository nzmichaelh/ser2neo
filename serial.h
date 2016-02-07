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
#pragma once

#include <stdint.h>

#include "chip.h"

/// Blocking serial port.
///
class Serial {
   public:
    void init();

    void putch(uint8_t ch);
    void putstr(const char* str);

    uint8_t getch();

   private:
    static const int ClockDiv = 1;

    uint32_t mem_[40 / sizeof(uint32_t)];
    UART_HANDLE_T* handle_;
};
