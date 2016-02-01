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
"""Demo that shows a basic analogue clock."""
import ser2neo
import datetime
import time


def angle2idx(neo, angle, limit):
    return round(angle * neo.nleds / limit) % neo.nleds


def main():
    port = ser2neo.open_port()
    neo = ser2neo.NeoPixels(port)

    anim = ser2neo.Animator()
    while anim.run_for(5):
        now = datetime.datetime.now()

        neo.clear()
        neo.set(angle2idx(neo, now.hour, 12), ser2neo.Colour.Blue)
        neo.set(angle2idx(neo, now.minute, 60), ser2neo.Colour.Red)

        # The quarter-hour ticks.
        top = ser2neo.Colour.Purple / 10
        mark = top / 4
        neo.set(angle2idx(neo, 0, 360), top)
        neo.set(angle2idx(neo, 90, 360), mark)
        neo.set(angle2idx(neo, 180, 360), mark)
        neo.set(angle2idx(neo, 270, 360), mark)

        neo.send()
        neo.wait()
        time.sleep(1)

if __name__ == '__main__':
    main()
