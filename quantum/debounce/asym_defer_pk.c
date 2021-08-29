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
Asymmetric per-key debounce algorithm. Allocates a byte per row and a byte
per key.

#define DEBOUNCE_DOWN and DEBOUNCE_UP. If not specified, they default to DEBOUNCE.
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

#ifndef DEBOUNCE_DOWN
#    define DEBOUNCE_DOWN DEBOUNCE
#endif

#ifndef DEBOUNCE_UP
#    define DEBOUNCE_UP DEBOUNCE
#endif

#ifndef DEBOUNCE_MUTE
#    define DEBOUNCE_MUTE 20
#endif

#define DEBOUNCE_EVENT_LOG_LEN 100

typedef struct asym_defer_state_t {
    uint8_t count;
} asym_defer_state_t;

static uint16_t last_time;
// [row][column] milliseconds until key's state is considered debounced.
static asym_defer_state_t* state;
// [row] number of keys in row currently debouncing. Used to efficiently
// skip rows.
static uint8_t* row_counts;

typedef enum debounce_event_type {
    NOTHING,
    DETECT_DOWN,
    DETECT_UP,
    COMMIT_DOWN,
    COMMIT_UP,
} debounce_event_type;

static const char* event_type_labels[] = {
    "",
    "detect-dn",
    "detect-up",
    "commit-dn",
    "commit-up",
};

typedef struct debounce_event_t {
    uint16_t now;
    uint8_t row;
    uint8_t col;
    debounce_event_type type;
} debounce_event_t;

static debounce_event_t debounce_event_log[DEBOUNCE_EVENT_LOG_LEN];
static int debounce_event_log_idx = 0;

static inline void print_event(debounce_event_t* evt) {
    const char* label = event_type_labels[evt->type];
    uprintf("%5u: %s at (%2u, %2u)\n", evt->now, label, evt->row, evt->col);
}

static inline void record_event(uint16_t now, uint8_t row, uint8_t col, debounce_event_type type) {
    debounce_event_t* evt = debounce_event_log + debounce_event_log_idx;
    debounce_event_log_idx++;
    if (debounce_event_log_idx == DEBOUNCE_EVENT_LOG_LEN) {
        debounce_event_log_idx = 0;
    }
    evt->now = now;
    evt->row = row;
    evt->col = col;
    evt->type = type;

#ifdef DEBUG_DEBOUNCE_RECORD
    print_event(evt);
#endif
}

void debounce_init(uint8_t num_rows) {
    state      = (asym_defer_state_t*)calloc(num_rows, MATRIX_COLS * sizeof(asym_defer_state_t));
    row_counts = (uint8_t*)calloc(num_rows, sizeof(uint8_t));

    last_time = timer_read();
}

void debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    uint16_t now       = timer_read();
    uint16_t elapsed16 = TIMER_DIFF_16(now, last_time);
    last_time          = now;
    uint8_t elapsed    = (elapsed16 > 255) ? 255 : elapsed16;

    asym_defer_state_t* statep = state;

    for (uint8_t row = 0; row < num_rows; ++row) {
        if (likely(row_counts[row] == 0) && !changed) {
            statep += MATRIX_COLS;
            continue;
        }

        local_row_t raw_row    = raw[row];
        local_row_t cooked_row = cooked[row];
        local_row_t delta      = raw_row ^ cooked_row;

        local_row_t col_mask = 1;
        for (uint8_t col = 0; col < MATRIX_COLS; ++col, col_mask <<= 1, ++statep) {
            if (unlikely(statep->count > elapsed)) {
                statep->count -= elapsed;
            } else if (unlikely(statep->count)) {
                if (delta & col_mask) {
                    cooked_row ^= col_mask;
                    statep->count = DEBOUNCE_MUTE;
                    record_event(now, row, col, (cooked_row & col_mask) ? COMMIT_DOWN : COMMIT_UP);
                } else {
                    statep->count = 0;
                    --row_counts[row];
                }
            } else if (changed && (delta & col_mask)) {
                record_event(now, row, col, (raw_row & col_mask) ? DETECT_DOWN : DETECT_UP);
                ++row_counts[row];
                statep->count = (raw_row & col_mask) ? DEBOUNCE_DOWN : DEBOUNCE_UP;
            }
        }

        cooked[row] = cooked_row;
    }
}

bool debounce_active(void) { return true; }

void debounce_debug(void) {
#ifdef CONSOLE_ENABLE
    int i = debounce_event_log_idx;
    int c = DEBOUNCE_EVENT_LOG_LEN;
    debounce_event_t* evt;
    
    while (c--) {
        evt = debounce_event_log + i;
        i++;
        if (i == DEBOUNCE_EVENT_LOG_LEN) {
            i = 0;
        }
        if (evt->type == NOTHING) {
            continue;
        }
        print_event(evt);
    }
#endif
}
