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
        }

        if (frame_count == 60)
        {
            draw_keyboard_update (current_key, false);
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
