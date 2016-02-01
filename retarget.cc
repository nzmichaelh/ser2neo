#include "chip.h"

extern "C" void _exit(int code) {
    for (;;) {
        __WFI();
    }
}

extern "C" void SystemInit() {
}
