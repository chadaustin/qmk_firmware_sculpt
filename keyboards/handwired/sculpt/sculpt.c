#include "sculpt.h"

#include <avr/power.h>

void keyboard_pre_init_kb(void) {
    keyboard_pre_init_user();

    // The default fuse bit from the factory enables CLKDIV,
    // so enable full frequency in software.
    clock_prescale_set(clock_div_1);
}
