/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#include "../tile_data/pattern_index.h"

const uint8_t keyboard_upper_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12,
    PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18, PATTERN_KEYS + 12
};

const uint8_t keyboard_upper_inactive [12] = {
    PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8,
    PATTERN_KEYS + 4, PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4,
    PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8, PATTERN_KEYS + 4
};

const uint8_t keyboard_mid_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12,
    PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20, PATTERN_KEYS + 12
};

const uint8_t keyboard_lower_inactive [12] = {
    PATTERN_KEYS + 2, PATTERN_KEYS + 3, PATTERN_KEYS + 4, PATTERN_KEYS + 5,
    PATTERN_KEYS + 4, PATTERN_KEYS + 2, PATTERN_KEYS + 6, PATTERN_KEYS + 4,
    PATTERN_KEYS + 7, PATTERN_KEYS + 4, PATTERN_KEYS + 5, PATTERN_KEYS + 4
};

const bool key_extends_left [12] = {
    false, false, true, false, true,
    false, false, true, false, true, false, true
};

const bool key_extends_right [12] = {
    true, false, true, false, false,
    true, false, true, false, true, false, false
};


/*
 * Initialise the keyboard display.
 */
void draw_keyboard (void)
{
    uint8_t key_tile;

    for (uint8_t col = 2; col <= 30; col++)
    {
        /* Top outline */
        SMS_setTileatXY (col, 16, PATTERN_KEYS + 0);

        /* Top section of keys */
        key_tile = keyboard_upper_inactive [(col - 2) % 12];
        SMS_setTileatXY (col, 17, key_tile);
        SMS_setTileatXY (col, 18, key_tile);
        SMS_setTileatXY (col, 19, key_tile);

        /* Bottom section of keys */
        key_tile = keyboard_lower_inactive [(col - 2) % 12];
        SMS_setTileatXY (col, 20, key_tile);
        SMS_setTileatXY (col, 21, key_tile);

        /* Bottom outline */
        SMS_setTileatXY (col, 22, PATTERN_KEYS + 16);
    }

    /* Right outline */
    SMS_setTileatXY (31, 16, PATTERN_KEYS + 1);
    for (uint8_t row = 17; row <= 21; row++)
    {
        SMS_setTileatXY (31, row, PATTERN_KEYS + 9);
    }
    SMS_setTileatXY (31, 22, PATTERN_KEYS + 17);
}


/*
 * Update the display of a key to be either active or inactive.
 * Assumes that only one key will ever be active at a time.
 */
void draw_keyboard_update (uint8_t key, bool active)
{
    uint8_t key_tile;

    /* Top section */
    if (active)
    {
        key_tile = keyboard_upper_active [key % 12];
    }
    else
    {
        key_tile = keyboard_upper_inactive [key % 12];
    }
    SMS_setTileatXY (key + 2, 17, key_tile);
    SMS_setTileatXY (key + 2, 18, key_tile);

    /* Mid section */
    if (active)
    {
        key_tile = keyboard_mid_active [key % 12];
    }
    SMS_setTileatXY (key + 2, 19, key_tile);

    /* Bottom section */
    if (key_extends_left [key % 12] || key_extends_right [key % 12])
    {
        if (active)
        {
            switch (key % 12)
            {
                case 0:
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 11);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 11);
                    break;
                case 2:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 19);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 19);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 13);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 13);
                    break;
                case 4:
                case 11:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 21);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 21);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    break;
                case 5:
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 14);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 14);
                    break;
                case 7:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 22);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 22);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 15);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 15);
                    break;
                case 9:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 23);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 23);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 13);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 13);
                    break;
            }
        }
        else
        {
            uint8_t from = ((key_extends_left  [key % 12]) ? key - 1 : key) + 2;
            uint8_t to   = ((key_extends_right [key % 12]) ? key + 1 : key) + 2;

            for (uint8_t col = from; col <= to; col++)
            {
                key_tile = keyboard_lower_inactive [(col - 2) % 12];
                SMS_setTileatXY (col, 20, key_tile);
                SMS_setTileatXY (col, 21, key_tile);
            }
        }
    }
}


/*
 * Draw the control labels.
 */
void draw_labels (void)
{
    /* Modulator / Carrier headings */
    const uint16_t mod_car_headings [] = {
        PATTERN_LABELS +  0, PATTERN_LABELS + 17, PATTERN_LABELS + 34, PATTERN_LABELS + 51,
        PATTERN_LABELS + 68, PATTERN_LABELS + 85, PATTERN_LABELS + 18, PATTERN_LABELS + 35,
        PATTERN_LABELS + 52, PATTERN_LABELS + 69, PATTERN_LABELS + 86 };
    SMS_loadTileMapArea (1, 4, mod_car_headings, 1, 11);

    /* Inst, vol, sust */
    const uint16_t inst_vol_sust [] = {
        PATTERN_LABELS + 1, PATTERN_LABELS + 2, PATTERN_EMPTY,
        PATTERN_LABELS + 3, PATTERN_LABELS + 4, PATTERN_EMPTY,
        PATTERN_LABELS + 5, PATTERN_LABELS + 6
    };
    SMS_loadTileMapArea (4, 3, inst_vol_sust, 8, 1);

    /* Feedback, total-level */
    const uint16_t feedback_tl [] = {
        PATTERN_LABELS + 42, PATTERN_LABELS + 43, PATTERN_LABELS + 44, PATTERN_LABELS + 45,
        PATTERN_LABELS + 46, PATTERN_LABELS + 47, PATTERN_LABELS + 59, PATTERN_LABELS + 60,
        PATTERN_LABELS + 61, PATTERN_LABELS + 62, PATTERN_LABELS + 63, PATTERN_LABELS + 64
    };
    SMS_loadTileMapArea (25, 3, feedback_tl, 6, 2);

    /* Multi */
    const uint16_t multi [] = {
        PATTERN_LABELS +  7, PATTERN_LABELS +  8, PATTERN_LABELS +  9, PATTERN_LABELS + 10,
        PATTERN_LABELS + 19, PATTERN_LABELS + 20, PATTERN_LABELS + 21, PATTERN_LABELS + 22,
    };
    SMS_loadTileMapArea (3,  8, &multi [0], 4, 1);
    SMS_loadTileMapArea (3, 13, &multi [4], 4, 1);

    /* KSR, vibrato, eg-type, AM, waveform */
    const uint16_t bit_labels [] = {
        PATTERN_LABELS + 11, PATTERN_LABELS + 12, PATTERN_EMPTY,       PATTERN_LABELS + 13,
        PATTERN_LABELS + 14, PATTERN_LABELS + 36, PATTERN_LABELS + 37, PATTERN_LABELS + 38,
        PATTERN_LABELS + 39, PATTERN_LABELS + 40, PATTERN_LABELS + 41, PATTERN_LABELS + 27,
        PATTERN_LABELS + 28, PATTERN_LABELS + 29, PATTERN_LABELS + 30, PATTERN_LABELS + 31,
        PATTERN_LABELS + 32,

        PATTERN_LABELS + 23, PATTERN_LABELS + 24, PATTERN_EMPTY,       PATTERN_LABELS + 25,
        PATTERN_LABELS + 26, PATTERN_LABELS + 53, PATTERN_LABELS + 54, PATTERN_LABELS + 55,
        PATTERN_LABELS + 56, PATTERN_LABELS + 57, PATTERN_LABELS + 58, PATTERN_LABELS + 48,
        PATTERN_LABELS + 49, PATTERN_LABELS + 50, PATTERN_LABELS + 65, PATTERN_LABELS + 66,
        PATTERN_LABELS + 67
    };
    SMS_loadTileMapArea (7,   7, &bit_labels [ 0], 5, 1);
    SMS_loadTileMapArea (6,   9, &bit_labels [ 5], 6, 1);
    SMS_loadTileMapArea (13,  7, &bit_labels [11], 3, 2);

    SMS_loadTileMapArea (7,  12, &bit_labels [17], 5, 1);
    SMS_loadTileMapArea (6,  14, &bit_labels [22], 6, 1);
    SMS_loadTileMapArea (13, 12, &bit_labels [28], 3, 2);

    /* KSL, A.rate, D.rate, S.level, R.rate */
    const uint16_t ksl_adsr [] = {
        PATTERN_LABELS + 70, PATTERN_LABELS + 71, PATTERN_LABELS + 72, PATTERN_LABELS + 73,
        PATTERN_LABELS + 74, PATTERN_LABELS + 75, PATTERN_LABELS + 76, PATTERN_LABELS + 77,
        PATTERN_LABELS + 78, PATTERN_LABELS + 79, PATTERN_LABELS + 80, PATTERN_LABELS + 81,
        PATTERN_LABELS + 82, PATTERN_LABELS + 83, PATTERN_LABELS + 84,
        PATTERN_LABELS + 87, PATTERN_LABELS + 88, PATTERN_LABELS + 89, PATTERN_LABELS + 90,
        PATTERN_LABELS + 91, PATTERN_LABELS + 92, PATTERN_LABELS + 93, PATTERN_LABELS + 94,
        PATTERN_LABELS + 95, PATTERN_LABELS + 96, PATTERN_LABELS + 97, PATTERN_LABELS + 98,
        PATTERN_LABELS + 99, PATTERN_LABELS + 100, PATTERN_LABELS + 101
    };
    SMS_loadTileMapArea (16,  8, &ksl_adsr [ 0], 15, 1);
    SMS_loadTileMapArea (16, 13, &ksl_adsr [15], 15, 1);
}


/*
 * Fill the name table with tile-zero.
 */
void draw_reset (void)
{
    uint16_t blank_line [32] = { 0 };

    for (uint8_t row = 0; row < 24; row++)
    {
        SMS_loadTileMapArea (0, row, blank_line, 32, 1);
    }
}


/*
 * Draw a two digit value indicator.
 */
void draw_value (uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t digit_1 = value / 10;
    uint8_t digit_2 = value % 10;

    /* First digit */
    if (digit_1 == 0)
    {
        digit_1 = 10;
    }
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + digit_1);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + digit_1 + 11);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + digit_2);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + digit_2 + 11);

}