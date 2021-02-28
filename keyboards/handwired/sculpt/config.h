/*
Copyright 2020 Michael Fincham
Copyright 2021 Chad Austin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x1209 // https://pid.codes/1209/
#define PRODUCT_ID      0xC846 // made up
#define DEVICE_VER      0x0001
#define MANUFACTURER    Chad Austin
#define PRODUCT         Wired Sculpt
#define DESCRIPTION     Microsoft Sculpt Wired Conversion

#define MATRIX_ROWS 8
#define MATRIX_COLS 18

#define MATRIX_ROW_PINS {F5, F6, F7, A0, A1, A2, A3, A4}
#define MATRIX_COL_PINS {A5, A6, A7, C7, C6, C5, C4, C3, C2, C1, C0, E1, E0, D7, D6, D5, D4, D3}
#define UNUSED_PINS {/*...*/}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCING_DELAY 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
#define MATRIX_HAS_GHOST


/*
 * Magic Key Options
 *
 * Magic keys are hotkey commands that allow control over firmware functions of
 * the keyboard. They are best used in combination with the HID Listen program,
 * found here: https://www.pjrc.com/teensy/hid_listen.html
 *
 * The options below allow the magic key functionality to be changed. This is
 * useful if your keyboard/keypad is missing keys and you want magic key support.
 *
 */

/* key combination for magic key command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#define LATENCY_MODE_ENABLE 0
#define LATENCY_MODE_PIN D2

#if !LATENCY_MODE_ENABLE
#define LED_CAPS_LOCK_PIN D2
#endif

#define SCULPT_RIGHT_SPACE_KEY KC_SPC

#define USB_POLLING_INTERVAL_MS 2


/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#endif
