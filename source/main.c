/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#include "../tile_data/pattern.h"
#include "../tile_data/pattern_index.h"
#include "../tile_data/palette.h"


/*
 * Fill the name table with tile-zero.
 */
static void clear_background (void)
{
    uint16_t blank_line [32] = { 0 };

    for (uint8_t row = 0; row < 24; row++)
    {
        SMS_loadTileMapArea (0, row, &blank_line, 32, 1);
    }
}


/*
 * Get the appropriate tile for a key on the keyboard.
 */
static uint8_t keyboard_tile (uint8_t key, bool top)
{
    if (top)
    {
        switch (key % 12)
        {
            case 0:     case 5:
                return PATTERN_KEYS + 2;

            case 1:     case 3:
            case 6:     case 8:
            case 10:
                return PATTERN_KEYS + 7;

            default:
                return PATTERN_KEYS + 3;
        }
    }
    else
    {
        switch (key % 12)
        {
            case 0:     case 5:
                return PATTERN_KEYS + 2;

            case 1:     case 6:
                return PATTERN_KEYS + 4;

            case 2:     case 4:
            case 7:     case 9:
            case 11:
                return PATTERN_KEYS + 3;

            case 3:     case 10:
                return PATTERN_KEYS + 5;

            case 8:
                return PATTERN_KEYS + 6;
        }
    }
}


/*
 * Initialise the keyboard display.
 */
static void draw_keyboard (void)
{
    uint8_t key_tile;

    for (uint8_t col = 2; col <= 30; col++)
    {
        /* Top outline */
        SMS_setTileatXY (col, 16, PATTERN_KEYS + 0);

        /* Top section of keys */
        key_tile = keyboard_tile (col - 2, true);
        SMS_setTileatXY (col, 17, key_tile);
        SMS_setTileatXY (col, 18, key_tile);
        SMS_setTileatXY (col, 19, key_tile);

        /* Bottom section of keys */
        key_tile = keyboard_tile (col - 2, false);
        SMS_setTileatXY (col, 20, key_tile);
        SMS_setTileatXY (col, 21, key_tile);

        /* Bottom outline */
        SMS_setTileatXY (col, 22, PATTERN_KEYS + 14);
    }

    /* Right outline */
    SMS_setTileatXY (31, 16, PATTERN_KEYS + 1);
    for (uint8_t row = 17; row <= 21; row++)
    {
        SMS_setTileatXY (31, row, PATTERN_KEYS + 8);
    }
    SMS_setTileatXY (31, 22, PATTERN_KEYS + 15);
}


/*
 * Entry point.
 */
void main (void)
{
    /* Load palette */
    SMS_loadBGPalette (palette);
    SMS_loadSpritePalette (palette);
    SMS_setBackdropColor (0);

    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_setBGScrollX (-4);
    clear_background ();
    draw_keyboard ();

    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        /* Nothing to do yet */
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
