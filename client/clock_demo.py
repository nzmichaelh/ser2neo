"""Demo that shows a basic analogue clock."""
import ser2neo
import datetime
import time


def angle2idx(neo, angle, limit):
    return round(angle * neo.nleds / limit) % neo.nleds


def main():
    port = ser2neo.open_port('/dev/ttyUSB0')
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
