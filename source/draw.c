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
 * Fill the name table with tile-zero.
 */
void draw_reset (void)
{
    uint16_t blank_line [32] = { 0 };

    for (uint8_t row = 0; row < 24; row++)
    {
        SMS_loadTileMapArea (0, row, &blank_line, 32, 1);
    }
}
