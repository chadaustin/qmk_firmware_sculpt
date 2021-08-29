#include "sculpt.h"
#include "debounce.h"
#include <avr/power.h>

void keyboard_pre_init_kb(void) {
    keyboard_pre_init_user();

    // The default fuse bit from the factory enables CLKDIV,
    // so enable full frequency in software.
    clock_prescale_set(clock_div_1);

    // Power reduction. Doesn't save much. :(
    PRR0 = 0b10000101;
    PRR1 = 0b00000001;

    // Default the charge pump's EN to off
    PORTB &= ~1;
    DDRB |= 1;

#if LATENCY_MODE_ENABLE
    setPinOutput(LATENCY_MODE_PIN);
#endif
}

void keyboard_post_init_kb(void) {
    // This runs after dip_switch_init. We cannot rely on the internal
    // pull-up on the Fn switch pin because the pull-down is weak at about
    // 47K and the AT90USB's internal pull-up is between 20K and 50K.
    setPinInput(F4);

    debug_enable = true;

    keyboard_post_init_user();
}

void dip_switch_update_kb(uint8_t index, bool active) {
    switch (index) {
    case 0:
        if (active) {
            layer_off(1);
        } else {
            layer_on(1);
        }
    }
    dip_switch_update_user(index, active);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
#if LATENCY_MODE_ENABLE
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
#endif
    const uint8_t both_shifts = MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT);
    switch (keycode) {
    case KC_H:
        if ((get_mods() & both_shifts) == both_shifts) {
            debounce_debug();
            return false;
        }
    }

    return process_record_user(keycode, record);
}
