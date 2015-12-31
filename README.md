# ser2neo

## Overview
ser2neo makes it easier to drive a NeoPixel ring by
implementing the NeoPixel protocol in an ATTINY85 and
exposing it through a serial interface.

Note: This is not an official Google product.

## Hardware
This software was developed for a
[OLIMEXINO-85](https://www.olimex.com/Products/Duino/AVR/OLIMEXINO-85-ASM/)
ATTINY85 USB board with a
[Adafruit NeoPixel 16 ring](https://www.adafruit.com/products/1463).
It should work with any other
[Digispark](http://digistump.com/products/1) clone.  The
implementation is very speed and timing sensitive.  If it doesn't
work check that PCIntDelay is correct for your CPU, clock speed, and
bootloader.

Pins:

* PB0 is TXD
* PB1 is DOUT and connects to the NeoPixel DIN
* PB2 is RXD

Connect the pins to a 3.3V serial interface such as the
[FTDI serial cable](https://www.sparkfun.com/products/9717).

See `client/` for a simple client library and demo applications.

## Implementation
NeoPixels use a single wire, 800 kHz, non-return-to-zero
protocol.  This implementation uses Timer0 in PWM mode where each PWM
cycle is one bit.  The PWM duty is updated each cycle.

The serial interface is a half duplex, command/response based
protocol that's designed to be mildly computer friendly.  Data is
transferred at 57600,N,8,1.  Character
stuffing is used to escape the special values, which are '\r' for end
of frame and '+' for escape.

The commands are:

* `!` - sync.  Responds with `OK sync`.
* `?` - query. Responds with `OK` followed by the system ID, version,
type, LED count, and colour order.
* `l` - set LEDs.  Followed by the LED data with three bytes per RGB
LED.  Responds with `OK`.

-- Michael Hope <michaelh@juju.net.nz> <mlhx@google.com>
