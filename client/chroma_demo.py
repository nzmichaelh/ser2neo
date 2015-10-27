"""Demo that shows all the colours of the rainbow."""
import ser2neo
import colorsys


def main():
    port = ser2neo.open_port('/dev/ttyUSB0')
    neo = ser2neo.NeoPixels(port)

    offset = 0
    anim = ser2neo.Animator()
    while anim.run_for(5):
        neo.clear()

        for i in range(neo.nleds):
            hue = offset + i / neo.nleds
            hue %= 1
            neo.set(i, ser2neo.RGB(*colorsys.hsv_to_rgb(hue, 1, 0.1)))

        offset += 0.003
        neo.send()
        neo.wait()

if __name__ == '__main__':
    main()
