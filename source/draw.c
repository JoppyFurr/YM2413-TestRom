/*
 * YM2413 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#include "../tile_data/pattern_index.h"

const uint16_t keyboard_upper_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12,
    PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18, PATTERN_KEYS + 12
};

const uint16_t keyboard_upper_inactive [12] = {
    PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8,
    PATTERN_KEYS + 4, PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4,
    PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8, PATTERN_KEYS + 4
};

const uint16_t keyboard_mid_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12,
    PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20, PATTERN_KEYS + 12
};

const uint16_t keyboard_lower_inactive [12] = {
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
 * Draw a button indicator.
 */
void draw_button (uint8_t x, uint8_t y, bool value)
{
    const uint16_t button [8] = {
        PATTERN_BUTTON + 0, PATTERN_BUTTON + 1, PATTERN_BUTTON + 2, PATTERN_BUTTON + 3,
        PATTERN_BUTTON + 4, PATTERN_BUTTON + 5, PATTERN_BUTTON + 6, PATTERN_BUTTON + 7
    };

    SMS_loadTileMapArea (x, y, value ? &button [4] : &button [0], 2, 2);
}


/*
 * Draw the footer banner at the bottom of the screen.
 */
void draw_footer (void)
{
    const uint16_t name [5] = {
        PATTERN_FOOTER + 0, PATTERN_FOOTER + 1, PATTERN_FOOTER + 2,
        PATTERN_FOOTER + 3, PATTERN_FOOTER + 4
    };
    SMS_loadTileMapArea (1, 23, name, 5, 1);

    const uint16_t version [3] = {
        PATTERN_FOOTER + 5, PATTERN_FOOTER + 6, PATTERN_FOOTER + 7,
    };
    SMS_loadTileMapArea (29, 23, version, 3, 1);
}


/*
 * Initialise the keyboard display.
 */
void draw_keyboard (void)
{
    uint16_t key_tile;

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
    uint16_t key_tile;

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
 * Draw the control labels for melody mode.
 */
void draw_melody_labels (void)
{
    /* Modulator / Carrier headings */
    const uint16_t mod_car_headings [] = {
        PATTERN_MELODY_LABELS +  0, PATTERN_MELODY_LABELS + 17, PATTERN_MELODY_LABELS + 34, PATTERN_MELODY_LABELS + 51,
        PATTERN_MELODY_LABELS + 68, PATTERN_MELODY_LABELS + 85, PATTERN_MELODY_LABELS + 18, PATTERN_MELODY_LABELS + 35,
        PATTERN_MELODY_LABELS + 52, PATTERN_MELODY_LABELS + 69, PATTERN_MELODY_LABELS + 86 };
    SMS_loadTileMapArea (1, 4, mod_car_headings, 1, 11);

    /* Inst, vol, sust */
    const uint16_t inst_vol_sust [] = {
        PATTERN_MELODY_LABELS + 1, PATTERN_MELODY_LABELS + 2, PATTERN_EMPTY,
        PATTERN_MELODY_LABELS + 3, PATTERN_MELODY_LABELS + 4, PATTERN_EMPTY,
        PATTERN_MELODY_LABELS + 5, PATTERN_MELODY_LABELS + 6
    };
    SMS_loadTileMapArea (4, 4, inst_vol_sust, 8, 1);

    /* Feedback */
    const uint16_t feedback [] = {
        PATTERN_MELODY_LABELS + 42, PATTERN_MELODY_LABELS + 43, PATTERN_MELODY_LABELS + 44,
        PATTERN_MELODY_LABELS + 59, PATTERN_MELODY_LABELS + 60, PATTERN_MELODY_LABELS + 61
    };
    SMS_loadTileMapArea (21, 3, feedback, 3, 2);

    /* Total-level */
    const uint16_t total_level [] = {
        PATTERN_MELODY_LABELS + 45, PATTERN_MELODY_LABELS + 46, PATTERN_MELODY_LABELS + 47,
        PATTERN_MELODY_LABELS + 62, PATTERN_MELODY_LABELS + 63, PATTERN_MELODY_LABELS + 64
    };
    SMS_loadTileMapArea (27, 3, total_level, 3, 2);

    /* Multi */
    const uint16_t multi [] = {
        PATTERN_MELODY_LABELS +  7, PATTERN_MELODY_LABELS +  8, PATTERN_MELODY_LABELS +  9, PATTERN_MELODY_LABELS + 10,
        PATTERN_MELODY_LABELS + 19, PATTERN_MELODY_LABELS + 20, PATTERN_MELODY_LABELS + 21, PATTERN_MELODY_LABELS + 22,
    };
    SMS_loadTileMapArea (3,  8, &multi [0], 4, 1);
    SMS_loadTileMapArea (3, 13, &multi [4], 4, 1);

    /* KSR, vibrato, eg-type, AM, waveform */
    const uint16_t bit_labels [] = {
        PATTERN_MELODY_LABELS + 11, PATTERN_MELODY_LABELS + 12, PATTERN_EMPTY,              PATTERN_MELODY_LABELS + 13,
        PATTERN_MELODY_LABELS + 14, PATTERN_MELODY_LABELS + 36, PATTERN_MELODY_LABELS + 37, PATTERN_MELODY_LABELS + 38,
        PATTERN_MELODY_LABELS + 39, PATTERN_MELODY_LABELS + 40, PATTERN_MELODY_LABELS + 41, PATTERN_MELODY_LABELS + 27,
        PATTERN_MELODY_LABELS + 28, PATTERN_MELODY_LABELS + 29, PATTERN_MELODY_LABELS + 30, PATTERN_MELODY_LABELS + 31,
        PATTERN_MELODY_LABELS + 32,

        PATTERN_MELODY_LABELS + 23, PATTERN_MELODY_LABELS + 24, PATTERN_EMPTY,              PATTERN_MELODY_LABELS + 25,
        PATTERN_MELODY_LABELS + 26, PATTERN_MELODY_LABELS + 53, PATTERN_MELODY_LABELS + 54, PATTERN_MELODY_LABELS + 55,
        PATTERN_MELODY_LABELS + 56, PATTERN_MELODY_LABELS + 57, PATTERN_MELODY_LABELS + 58, PATTERN_MELODY_LABELS + 48,
        PATTERN_MELODY_LABELS + 49, PATTERN_MELODY_LABELS + 50, PATTERN_MELODY_LABELS + 65, PATTERN_MELODY_LABELS + 66,
        PATTERN_MELODY_LABELS + 67
    };
    SMS_loadTileMapArea (7,   7, &bit_labels [ 0], 5, 1);
    SMS_loadTileMapArea (6,   9, &bit_labels [ 5], 6, 1);
    SMS_loadTileMapArea (13,  7, &bit_labels [11], 3, 2);

    SMS_loadTileMapArea (7,  12, &bit_labels [17], 5, 1);
    SMS_loadTileMapArea (6,  14, &bit_labels [22], 6, 1);
    SMS_loadTileMapArea (13, 12, &bit_labels [28], 3, 2);

    /* KSL, A.rate, D.rate, S.level, R.rate */
    const uint16_t ksl_adsr [] = {
        PATTERN_MELODY_LABELS + 70, PATTERN_MELODY_LABELS +  71, PATTERN_MELODY_LABELS +  72, PATTERN_MELODY_LABELS + 73,
        PATTERN_MELODY_LABELS + 74, PATTERN_MELODY_LABELS +  75, PATTERN_MELODY_LABELS +  76, PATTERN_MELODY_LABELS + 77,
        PATTERN_MELODY_LABELS + 78, PATTERN_MELODY_LABELS +  79, PATTERN_MELODY_LABELS +  80, PATTERN_MELODY_LABELS + 81,
        PATTERN_MELODY_LABELS + 82, PATTERN_MELODY_LABELS +  83, PATTERN_MELODY_LABELS +  84,
        PATTERN_MELODY_LABELS + 87, PATTERN_MELODY_LABELS +  88, PATTERN_MELODY_LABELS +  89, PATTERN_MELODY_LABELS + 90,
        PATTERN_MELODY_LABELS + 91, PATTERN_MELODY_LABELS +  92, PATTERN_MELODY_LABELS +  93, PATTERN_MELODY_LABELS + 94,
        PATTERN_MELODY_LABELS + 95, PATTERN_MELODY_LABELS +  96, PATTERN_MELODY_LABELS +  97, PATTERN_MELODY_LABELS + 98,
        PATTERN_MELODY_LABELS + 99, PATTERN_MELODY_LABELS + 100, PATTERN_MELODY_LABELS + 101
    };
    SMS_loadTileMapArea (16,  8, &ksl_adsr [ 0], 15, 1);
    SMS_loadTileMapArea (16, 13, &ksl_adsr [15], 15, 1);
}


/*
 * Draw the control labels for melody mode.
 */
void draw_rhythm_labels (void)
{
    /* Sustain, Block, F-num */
    const uint16_t block_fnum [] = {
        PATTERN_RHYTHM_LABELS + 0, PATTERN_RHYTHM_LABELS + 1, PATTERN_RHYTHM_LABELS + 2, PATTERN_RHYTHM_LABELS + 3,
        PATTERN_RHYTHM_LABELS + 4, PATTERN_RHYTHM_LABELS + 5, PATTERN_RHYTHM_LABELS + 6, PATTERN_RHYTHM_LABELS + 7,
        PATTERN_RHYTHM_LABELS + 8,
    };
    SMS_loadTileMapArea (5, 6, block_fnum, 9, 1);
    SMS_loadTileMapArea (5, 12, block_fnum, 9, 1);
    SMS_loadTileMapArea (5, 20, block_fnum, 9, 1);

    /* Volume */
    const uint16_t volume [] = { PATTERN_RHYTHM_LABELS + 9, PATTERN_RHYTHM_LABELS + 10 };
    SMS_loadTileMapArea (17,  6, volume, 2, 1);
    SMS_loadTileMapArea (17, 10, volume, 2, 1);
    SMS_loadTileMapArea (17, 14, volume, 2, 1);
    SMS_loadTileMapArea (17, 18, volume, 2, 1);
    SMS_loadTileMapArea (17, 22, volume, 2, 1);

    /* Bass Drum */
    const uint16_t bass_drum [] = {
        PATTERN_RHYTHM_LABELS + 11, PATTERN_RHYTHM_LABELS + 12, PATTERN_RHYTHM_LABELS + 13, PATTERN_RHYTHM_LABELS + 14,
        PATTERN_RHYTHM_LABELS + 15
    };
    SMS_loadTileMapArea (24, 4, bass_drum, 5, 1);

    /* High Hat */
    const uint16_t high_hat [] = {
        PATTERN_RHYTHM_LABELS + 16, PATTERN_RHYTHM_LABELS + 17, PATTERN_RHYTHM_LABELS + 18, PATTERN_RHYTHM_LABELS + 19
    };
    SMS_loadTileMapArea (24, 8, high_hat, 4, 1);

    /* Snare Drum */
    const uint16_t snare_drum [] = {
        PATTERN_RHYTHM_LABELS + 20, PATTERN_RHYTHM_LABELS + 21, PATTERN_RHYTHM_LABELS + 22, PATTERN_RHYTHM_LABELS + 23,
        PATTERN_RHYTHM_LABELS + 24, PATTERN_RHYTHM_LABELS + 25
    };
    SMS_loadTileMapArea (24, 12, snare_drum, 6, 1);

    /* Tom Tom */
    const uint16_t tom_tom [] = {
        PATTERN_RHYTHM_LABELS + 26, PATTERN_RHYTHM_LABELS + 27, PATTERN_RHYTHM_LABELS + 28, PATTERN_RHYTHM_LABELS + 29
    };
    SMS_loadTileMapArea (24, 16, tom_tom, 4, 1);

    /* Top Cymbal */
    const uint16_t top_cymbal [] = {
        PATTERN_RHYTHM_LABELS + 30, PATTERN_RHYTHM_LABELS + 31, PATTERN_RHYTHM_LABELS + 32, PATTERN_RHYTHM_LABELS + 33,
        PATTERN_RHYTHM_LABELS + 34, PATTERN_RHYTHM_LABELS + 35
    };
    SMS_loadTileMapArea (24, 20, top_cymbal, 6, 1);

    /* Connecting lines */
    const uint16_t lines [] = {
        PATTERN_RHYTHM_LABELS + 36, PATTERN_RHYTHM_LABELS + 37, PATTERN_RHYTHM_LABELS + 38, /* --- */
        PATTERN_RHYTHM_LABELS + 36, PATTERN_RHYTHM_LABELS + 38,                             /* --  */
        PATTERN_RHYTHM_LABELS + 39, PATTERN_RHYTHM_LABELS + 40, PATTERN_RHYTHM_LABELS + 39, /* ||| */
        PATTERN_RHYTHM_LABELS + 41, PATTERN_RHYTHM_LABELS + 38                              /* L-  */
    };

    SMS_loadTileMapArea (14,  4, &lines [0], 3, 1);
    SMS_loadTileMapArea (19,  4, &lines [3], 2, 1);

    SMS_setTileatXY (14, 10, PATTERN_RHYTHM_LABELS + 36);
    SMS_loadTileMapArea (15,  8, &lines [3], 2, 1);
    SMS_loadTileMapArea (15,  9, &lines [5], 1, 3);
    SMS_loadTileMapArea (15, 12, &lines [8], 2, 1);
    SMS_loadTileMapArea (19,  8, &lines [3], 2, 1);
    SMS_loadTileMapArea (19, 12, &lines [3], 2, 1);

    SMS_setTileatXY (14, 18, PATTERN_RHYTHM_LABELS + 36);
    SMS_loadTileMapArea (15, 16, &lines [3], 2, 1);
    SMS_loadTileMapArea (15, 17, &lines [5], 1, 3);
    SMS_loadTileMapArea (15, 20, &lines [8], 2, 1);
    SMS_loadTileMapArea (19, 16, &lines [3], 2, 1);
    SMS_loadTileMapArea (19, 20, &lines [3], 2, 1);
}


/*
 * Draw an LED indicator.
 */
void draw_led (uint8_t x, uint8_t y, bool value)
{
    if (value)
    {
        SMS_setTileatXY (x,     y, PATTERN_LED + 2);
        SMS_setTileatXY (x + 1, y, PATTERN_LED + 3);
    }
    else
    {
        SMS_setTileatXY (x,     y, PATTERN_LED + 0);
        SMS_setTileatXY (x + 1, y, PATTERN_LED + 1);
    }
}


/*
 * Draw the name of the selected instrument.
 */
void draw_instrument_name (uint8_t instrument)
{
    const uint8_t bases [16] =   {0, 4, 7, 10, 13, 16, 20, 22, 26, 29, 32, 38, 44, 49, 57, 62};
    const uint8_t lengths [16] = {4, 3, 3,  3,  3,  4,  2,  4,  3,  3,  6,  6,  5,  8,  5,  7};

    uint16_t name_tiles [8];
    uint16_t name_base = PATTERN_NAMES + bases [instrument];
    uint8_t name_len = lengths [instrument];


    for (uint8_t i = 0; i < 8; i++)
    {
        if (i < name_len)
        {
            name_tiles [i] = name_base + i;
        }
        else
        {
            name_tiles [i] = PATTERN_KEYS + 0;
        }
    }

    SMS_loadTileMapArea (2, 16, name_tiles, 8, 1);
}


/*
 * Fill the name table with tile-zero.
 */
void draw_reset (uint8_t from, uint8_t to)
{
    uint16_t blank_line [32] = { 0 };

    for (uint8_t row = from; row < to; row++)
    {
        SMS_loadTileMapArea (0, row, blank_line, 32, 1);
    }
}


/*
 * Draw the Melody/Rhythm tabs.
 */
void draw_tabs (bool rhythm)
{
    const uint16_t melody_tiles [18] =  {
        PATTERN_TABS +  0, PATTERN_TABS +  1, PATTERN_TABS +  2, PATTERN_TABS +  3,
        PATTERN_TABS +  4, PATTERN_TABS +  5, PATTERN_TABS +  6, PATTERN_TABS +  7,
        PATTERN_TABS +  8, PATTERN_TABS +  9, PATTERN_TABS + 10, PATTERN_TABS + 11,
        PATTERN_TABS + 12, PATTERN_TABS + 13, PATTERN_TABS + 14, PATTERN_TABS + 15,
        PATTERN_TABS + 16, PATTERN_TABS + 17
    };
    const uint16_t rhythm_tiles [18] =  {
        PATTERN_TABS + 18, PATTERN_TABS + 19, PATTERN_TABS + 20, PATTERN_TABS + 21,
        PATTERN_TABS + 22, PATTERN_TABS + 23, PATTERN_TABS + 24, PATTERN_TABS + 25,
        PATTERN_TABS + 26, PATTERN_TABS + 27, PATTERN_TABS + 28, PATTERN_TABS + 29,
        PATTERN_TABS + 30, PATTERN_TABS + 31, PATTERN_TABS + 32, PATTERN_TABS + 33,
        PATTERN_TABS + 34, PATTERN_TABS + 35
    };

    SMS_loadTileMapArea (23, 0, rhythm ? rhythm_tiles : melody_tiles, 9, 2);
}


/*
 * Draw the title text, "YM2413 TestRom"
 */
void draw_title (void)
{
    uint16_t title [20] = { PATTERN_TITLE +  0, PATTERN_TITLE +  1, PATTERN_TITLE +  2, PATTERN_TITLE +  3,
                            PATTERN_TITLE +  4, PATTERN_TITLE +  5, PATTERN_TITLE +  6, PATTERN_TITLE +  7,
                            PATTERN_TITLE +  8, PATTERN_TITLE +  9, PATTERN_TITLE + 10, PATTERN_TITLE + 11,
                            PATTERN_TITLE + 12, PATTERN_TITLE + 13, PATTERN_TITLE + 14, PATTERN_TITLE + 15,
                            PATTERN_TITLE + 16, PATTERN_TITLE + 17, PATTERN_TITLE + 18, PATTERN_TITLE + 19 };

    SMS_loadTileMapArea (11, 0, title, 10, 2);
}


/*
 * Draw a two digit value indicator.
 */
void draw_value (uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t digit_1 = value / 10;
    uint8_t digit_2 = value % 10;

    /* Don't show leading zeros */
    if (digit_1 == 0)
    {
        digit_1 = 10;
    }

    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + digit_1);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + digit_1 + 11);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + digit_2);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + digit_2 + 11);

}


/*
 * Draw a three digit value indicator.
 */
void draw_value_wide (uint8_t x, uint8_t y, uint16_t value)
{
    uint8_t digit_1 = value / 100;
    uint8_t digit_2 = (value % 100) / 10;
    uint8_t digit_3 = value % 10;

    /* Don't show leading zeros */
    if (digit_1 == 0)
    {
        digit_1 = 10;

        if (digit_2 == 0)
        {
            digit_2 = 10;
        }
    }

    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + digit_1);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + digit_1 + 11);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + digit_2);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + digit_2 + 11);

    /* Third digit */
    SMS_setTileatXY (x + 2, y,     PATTERN_DIGITS + digit_3);
    SMS_setTileatXY (x + 2, y + 1, PATTERN_DIGITS + digit_3 + 11);

}


/*
 * Draw a dimmed-out two digit value indicator.
 */
void draw_value_hidden (uint8_t x, uint8_t y)
{
    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + 10);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + 21);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + 10);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + 21);
}
