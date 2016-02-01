# Copyright 2015 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""Serial to NeoPixel client side driver."""
from __future__ import division

import serial
import serial.tools.list_ports
import time
import struct


def _clip(v, top=1.0):
    """Ensure a value is in the range [0, top]."""
    return max(0, min(top, v))


class RGB:
    """A single colour with basic addition and blend support."""

    def __init__(self, r, g, b):
        self.rgb = (r, g, b)

    def __add__(self, other):
        """Componentwise addition."""
        return RGB(*list(_clip(x + y) for x, y in zip(self.rgb, other.rgb)))

    def __mul__(self, level):
        """Scalar multiplier."""
        return RGB(*(_clip(x * level) for x in self.rgb))

    def __truediv__(self, level):
        """Scalar division."""
        return RGB(*(_clip(x / level) for x in self.rgb))

    __div__ = __truediv__

    def __repr__(self):
        return '<RGB %.2f %.2f %.2f>' % self.rgb


class Colour:
    """Standard colours."""
    Black = RGB(0, 0, 0)
    White = RGB(1, 1, 1)
    Red = RGB(1, 0, 0)
    Green = RGB(0, 1, 0)
    Blue = RGB(0, 0, 1)
    Purple = RGB(1, 0, 1)


def _escape(buf):
    EOL, ESC = ord('\r'), ord('+')
    special = set((EOL, ESC, ord('?')))

    for ch in buf:
        if ch in special:
            yield ESC
            yield ch ^ ESC
        else:
            yield ch


def first_serial():
    for info in serial.tools.list_ports.comports():
        device = info[0]
        if 'USB' in device or 'ACM' in device:
            return device
    return '/dev/ttyUSB0'


def open_port(name=None):
    """Open the serial connection to the adaptor."""
    if name is None:
        name = first_serial()
    return serial.Serial(name, 57600, timeout=0.02)


class NeoPixels:
    """Client side interface to the ser2neo NeoPixel adapter."""

    def __init__(self, port, nleds=16):
        self.port = port
        self.nleds = nleds
        self.brightness = 1
        self.clear()

    def clear(self):
        """Reset to black/off."""
        self.leds = [Colour.Black for i in range(self.nleds)]

    def set(self, at, rgb):
        """Set a LED, blending it with the existing colour if any.

        'at' is the index of the LED to set.  The LEDs are treated as a
        ring, which allows negative indexes and indexes greater than the
        number of LEDs in the ring.

        """
        idx = int(at + 0.5)
        idx %= len(self.leds)
        self.leds[idx] = self.leds[idx] + rgb

    def encode(self):
        """Encode into a ser2neo compatible byte buffer."""
        buf = b'l'
        for rgb in self.leds:
            rgb = rgb * self.brightness
            buf += struct.pack('BBB', int(_clip(rgb.rgb[1] * 0xFF, 0xFF)),
                               int(_clip(rgb.rgb[0] * 0xFF, 0xFF)),
                               int(_clip(rgb.rgb[2] * 0xFF, 0xFF)))
        if str == bytes:
            ints = (ord(x) for x in buf)
            buf = b''.join(chr(x) for x in _escape(ints))
        else:
            buf = bytes(_escape(buf))
        return buf + b'\r'

    def send(self):
        """Encode and send the current values."""
        self.port.write(self.encode())

    def wait(self):
        """Wait for the device to acknowledge the last command."""
        buf = b''
        while True:
            got = self.port.read(1)
            if not got:
                return False
            if got == b'\n':
                return True


class Animator:
    """A simple animation controller used in the demos."""

    def __init__(self):
        self.start = time.time()

    def run_for(self, secs):
        """Returns False if 'secs' has passed."""
        now = time.time()
        return (now - self.start) < secs
