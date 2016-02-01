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
"""Demo that spins a six sided dice."""
import ser2neo
import datetime
import time
import random


def angle2idx(neo, angle, limit):
    return round(angle * neo.nleds / limit)


def main():
    port = ser2neo.open_port()
    neo = ser2neo.NeoPixels(port)

    random.seed()
    dt = 0.02 + random.random() * 0.02
    gain = 0.05
    at = 1
    run = 0

    while dt < 0.4:
        neo.clear()
        for i in range(0, at):
            neo.set(angle2idx(neo, i, at) + run, ser2neo.Colour.Red)

        neo.send()
        neo.wait()

        at += 1
        if at == 7:
            at = 1
        # Also rotate the dice to make it look nicer.
        run += 1

        time.sleep(dt)
        # Slowly bring the dice to a stop.
        dt += dt * gain

    time.sleep(2)

if __name__ == '__main__':
    main()
