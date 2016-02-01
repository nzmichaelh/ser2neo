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
#pragma once

#include <stdint.h>

/// NeoPixel ring driver.
///
/// Implemented as a bit-banged loop.
///
class NeoPixel {
   public:
    NeoPixel() { clear(); }

    void init();

    void clear();
    void append(uint8_t b);
    void write();

   private:
    static const int MaxLEDs = 16;

    static const int ColoursPerLED = 3;
    
    void send(uint16_t ch);
    
    uint8_t* p_;
    uint8_t bytes_[MaxLEDs * ColoursPerLED];
    static const uint16_t bits_[];
};
