/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#include "../tile_data/palette.h"
#include "../tile_data/pattern.h"
#include "draw.h"


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
    SMS_setBGScrollX (0xfc);
    draw_reset ();
    draw_labels ();
    draw_keyboard ();

    draw_value ( 4, 1, 0); /* Inst */
    draw_value ( 7, 1, 1); /* Vol */
    draw_led   (10, 2, false); /* Sustain */
    draw_value (25, 1, 2); /* Feedback */
    draw_value (28, 1, 3); /* Total level */

    draw_value ( 4, 6, 4); /* Multi (mod) */
    draw_led   ( 7, 6, false); /* KSR */
    draw_led   (10, 6, false); /* Vibrato */
    draw_led   (13, 6, false); /* Waveform */
    draw_led   ( 7, 8, false); /* EG Type */
    draw_led   (10, 8, false); /* AM */
    draw_value (16, 6, 5); /* KSL (mod) */
    draw_value (19, 6, 6); /* A.Rate (mod) */
    draw_value (22, 6, 7); /* D.Rate (mod) */
    draw_value (25, 6, 8); /* S.Level (mod) */
    draw_value (28, 6, 9); /* R.Rate (mod) */

    draw_value ( 4, 11, 10); /* Multi (car) */
    draw_led   ( 7, 11, false); /* KSR */
    draw_led   (10, 11, false); /* Vibrato */
    draw_led   (13, 11, false); /* Waveform */
    draw_led   ( 7, 13, false); /* EG Type */
    draw_led   (10, 13, false); /* AM */
    draw_value (16, 11, 11); /* KSL (car) */
    draw_value (19, 11, 12); /* A.Rate (car) */
    draw_value (22, 11, 13); /* D.Rate (car) */
    draw_value (25, 11, 14); /* S.Lev (car) */
    draw_value (28, 11, 15); /* R.Rate (car) */

    SMS_displayOn ();

    /* Main loop */
    uint8_t frame_count = 0;
    uint8_t current_key = 0;
    while (true)
    {
        /* Basic test for key down indication - cycle through the keys. */
        SMS_waitForVBlank ();
        frame_count++;

        if (frame_count == 30)
        {
            draw_keyboard_update (current_key, true);
            draw_value (4, 1, current_key); /* Inst */
            draw_led   (10, 2, true); /* Sustain */
        }

        if (frame_count == 60)
        {
            draw_keyboard_update (current_key, false);
            draw_led   (10, 2, false); /* Sustain */
            frame_count = 0;
            current_key++;
            if (current_key > 28)
            {
                current_key = 0;
            }
        }

    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
