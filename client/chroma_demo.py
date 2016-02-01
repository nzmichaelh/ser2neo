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
"""Demo that shows all the colours of the rainbow."""
from __future__ import division

import ser2neo
import colorsys


def main():
    port = ser2neo.open_port()
    neo = ser2neo.NeoPixels(port)

    offset = 0
    anim = ser2neo.Animator()
    while anim.run_for(5):
        neo.clear()

        for i in range(neo.nleds):
            hue = offset + i / neo.nleds
            hue %= 1.0
            neo.set(i, ser2neo.RGB(*colorsys.hsv_to_rgb(hue, 1, 0.1)))

        offset += 0.003
        neo.send()
        neo.wait()

if __name__ == '__main__':
    main()
