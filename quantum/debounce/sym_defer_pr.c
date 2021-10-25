/*
Copyright 2017 Alex Ong<the.onga@gmail.com>
Copyright 2021 Chad Austin <chad@chadaustin.me>
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

/*
Symmetric per-row debounce algorithm. Changes only apply when
DEBOUNCE milliseconds have elapsed since the last change.
*/

#include "matrix.h"
#include "timer.h"
#include "quantum.h"
#include <stdlib.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

/*
 * Keyboards with more than 16 columns can save a significant number of
 * instructions on AVR by using 24-bit integers instead of 32-bit.
 */
#if (MATRIX_COLS > 16 && MATRIX_COLS <= 24)
typedef __uint24 local_row_t;
#else
typedef matrix_row_t local_row_t;
#endif

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

static uint16_t last_time;
// [row] milliseconds until key's state is considered debounced.
static uint8_t* countdowns;
// [row]
static local_row_t* last_raw;

void debounce_init(uint8_t num_rows) {
    countdowns      = (uint8_t*)calloc(num_rows, sizeof(uint8_t));
    last_raw = (local_row_t*)calloc(num_rows, sizeof(local_row_t));

    last_time = timer_read();
}

void debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    static bool initialized = false;
    if (!initialized) {
        for (uint8_t r = 0; r < num_rows; ++r) {
            last_raw[r] = raw[r];
        }
        initialized = true;
    }

    uint16_t now       = timer_read();
    uint16_t elapsed16 = TIMER_DIFF_16(now, last_time);
    last_time          = now;
    uint8_t elapsed    = (elapsed16 > 255) ? 255 : elapsed16;

    uint8_t* countdown = countdowns;

    for (uint8_t row = 0; row < num_rows; ++row, ++countdown) {
        local_row_t raw_row    = raw[row];

        if (raw_row != last_raw[row]) {
            *countdown = DEBOUNCE;
            last_raw[row] = raw_row;
        } else if (*countdown > elapsed) {
            *countdown -= elapsed;
        } else if (*countdown) {
            cooked[row] = raw_row;
            *countdown = 0;
        }
    }
}

bool debounce_active(void) { return true; }

void debounce_debug(void) {
}
