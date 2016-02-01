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
"""Demo that spins two green lights with trails."""
import ser2neo
import time


def main():
    port = ser2neo.open_port()
    neo = ser2neo.NeoPixels(port)

    at = 0
    anim = ser2neo.Animator()
    while anim.run_for(5):
        neo.clear()
        neo.set(at, ser2neo.Colour.Green)
        neo.set(at - 1, ser2neo.Colour.Green / 4)
        neo.set(at - 2, ser2neo.Colour.Green / 16)

        neo.set(at + neo.nleds / 2, ser2neo.Colour.Green)
        neo.set(at + neo.nleds / 2 - 1, ser2neo.Colour.Green / 4)
        neo.set(at + neo.nleds / 2 - 2, ser2neo.Colour.Green / 16)

        at += 1
        neo.send()
        neo.wait()

        time.sleep(0.05)

if __name__ == '__main__':
    main()
