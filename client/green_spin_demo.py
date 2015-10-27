"""Demo that spins two green lights with trails."""
import ser2neo
import time


def main():
    port = ser2neo.open_port('/dev/ttyUSB0')
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
