/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"


/*
 * Entry point.
 */
void main (void)
{
    /* Initialise background to dark gray */
    SMS_setSpritePaletteColor (0, 0x15);
    SMS_setBGPaletteColor     (0, 0x15);
    SMS_setBackdropColor (0);
    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        /* Nothing to do yet */
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
