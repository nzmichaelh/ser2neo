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
"""Demo that spins two lights with a short trail."""
import ser2neo


def main():
    port = ser2neo.open_port()
    neo = ser2neo.NeoPixels(port)

    p0 = 0
    p1 = 5

    anim = ser2neo.Animator()
    while anim.run_for(5):
        neo.clear()
        neo.set(p0, ser2neo.Colour.Red)
        neo.set(p0 + 1, ser2neo.Colour.Red / 4)
        neo.set(p0 + 2, ser2neo.Colour.Red / 16)
        p0 -= 0.5

        neo.set(p1, ser2neo.Colour.Blue)
        neo.set(p1 - 1, ser2neo.Colour.Blue / 4)
        neo.set(p1 - 2, ser2neo.Colour.Blue / 16)
        p1 += 0.2

        neo.send()
        neo.wait()

if __name__ == '__main__':
    main()
