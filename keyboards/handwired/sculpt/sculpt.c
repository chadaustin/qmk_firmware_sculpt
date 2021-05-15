#include "sculpt.h"

#include <avr/power.h>

                 void keyboard_pre_init_kb(void) {
    keyboard_pre_init_user();

    // The default fuse bit from the factory enables CLKDIV,
    // so enable full frequency in software.
    clock_prescale_set(clock_div_1);

    // Power reduction. Doesn't save much. :(
    PRR0 = 0b10000101;
    PRR1 = 0b00000001;

    PORTB &= ~1;
    DDRB |= 1;

#if LATENCY_MODE_ENABLE
    setPinOutput(LATENCY_MODE_PIN);
#endif
}


#if LATENCY_MODE_ENABLE
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    static int count = 0;
    if (record->event.pressed) {
        ++count;
#ifdef LATENCY_MODE_PIN
        writePinHigh(LATENCY_MODE_PIN);
#endif
    } else {
        --count;
        if (count < 0) {
            count = 0;
        }
        if (count == 0) {
            writePinLow(LATENCY_MODE_PIN);
        }
    }
    return process_record_user(keycode, record);
}
#endif
