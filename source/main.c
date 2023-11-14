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
#include "../tile_data/pattern_index.h"
#include "draw.h"

typedef enum element_type_e {
    TYPE_VALUE = 0,
    TYPE_LED
} element_type_t;

typedef enum element_id_e {
    ELEMENT_INSTRUMENT = 0,
    ELEMENT_VOLUME,
    ELEMENT_SUSTAIN,
    ELEMENT_FEEDBACK,
    ELEMENT_TOTAL_LEVEL,
    ELEMENT_MOD_MULTI,
    ELEMENT_MOD_KSR,
    ELEMENT_MOD_VIBRATO,
    ELEMENT_MOD_WAVEFORM,
    ELEMENT_MOD_ENVELOPE_TYPE,
    ELEMENT_MOD_AM,
    ELEMENT_MOD_KSL,
    ELEMENT_MOD_ATTACK_RATE,
    ELEMENT_MOD_DECAY_RATE,
    ELEMENT_MOD_SUSTAIN_LEVEL,
    ELEMENT_MOD_RELEASE_RATE,
    ELEMENT_CAR_MULTI,
    ELEMENT_CAR_KSR,
    ELEMENT_CAR_VIBRATO,
    ELEMENT_CAR_WAVEFORM,
    ELEMENT_CAR_ENVELOPE_TYPE,
    ELEMENT_CAR_AM,
    ELEMENT_CAR_KSL,
    ELEMENT_CAR_ATTACK_RATE,
    ELEMENT_CAR_DECAY_RATE,
    ELEMENT_CAR_SUSTAIN_LEVEL,
    ELEMENT_CAR_RELEASE_RATE,
    ELEMENT_KEYBOARD,
    ELEMENT_COUNT
} element_id_t;


typedef struct gui_element_s {

    element_type_t type;
    uint16_t max;

    uint8_t x;
    uint8_t y;
    uint8_t height;

    element_id_t up;
    element_id_t down;
    element_id_t left;
    element_id_t right;

} gui_element_t;

gui_element_t main_gui [ELEMENT_COUNT] = {
    [ELEMENT_INSTRUMENT] = {
        .type = TYPE_VALUE, .max = 15, .x = 4, .y = 1, .height = 3,
        .up   = ELEMENT_INSTRUMENT,         .down  = ELEMENT_MOD_MULTI,
        .left = ELEMENT_INSTRUMENT,         .right = ELEMENT_VOLUME
    },
    [ELEMENT_VOLUME] = {
        .type = TYPE_VALUE, .max = 15, .x = 7, .y = 1, .height = 3,
        .up   = ELEMENT_VOLUME,             .down  = ELEMENT_MOD_KSR,
        .left = ELEMENT_INSTRUMENT,         .right = ELEMENT_SUSTAIN
    },
    [ELEMENT_SUSTAIN] = {
        .type = TYPE_LED, .max = 1, .x = 10, .y = 2, .height = 2,
        .up   = ELEMENT_SUSTAIN,            .down  = ELEMENT_MOD_VIBRATO,
        .left = ELEMENT_VOLUME,             .right = ELEMENT_FEEDBACK
    },
    [ELEMENT_FEEDBACK] = {
        .type = TYPE_VALUE, .max = 7, .x = 25, .y = 1, .height = 4,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_MOD_SUSTAIN_LEVEL,
        .left = ELEMENT_SUSTAIN,            .right = ELEMENT_TOTAL_LEVEL
    },
    [ELEMENT_TOTAL_LEVEL] = {
        .type = TYPE_VALUE, .max = 63, .x = 28, .y = 1, .height = 4,
        .up   = ELEMENT_TOTAL_LEVEL,        .down  = ELEMENT_MOD_RELEASE_RATE,
        .left = ELEMENT_FEEDBACK,           .right = ELEMENT_TOTAL_LEVEL
    },
    [ELEMENT_MOD_MULTI] = {
        .type = TYPE_VALUE, .max = 15, .x = 4, .y = 6, .height = 3,
        .up   = ELEMENT_INSTRUMENT,         .down  = ELEMENT_CAR_MULTI,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_KSR
    },
    [ELEMENT_MOD_KSR] = {
        .type = TYPE_LED, .max = 1, .x = 7, .y = 6, .height = 2,
        .up   = ELEMENT_VOLUME,             .down  = ELEMENT_MOD_ENVELOPE_TYPE,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_VIBRATO
    },
    [ELEMENT_MOD_VIBRATO] = {
        .type  = TYPE_LED, .max = 1, .x = 10, .y = 6, .height = 2,
        .up    = ELEMENT_SUSTAIN,           .down  = ELEMENT_MOD_AM,
        .left  = ELEMENT_MOD_KSR,           .right = ELEMENT_MOD_WAVEFORM
    },
    [ELEMENT_MOD_WAVEFORM] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 6, .height = 3,
        .up   = ELEMENT_SUSTAIN,            .down  = ELEMENT_CAR_WAVEFORM,
        .left = ELEMENT_MOD_VIBRATO,        .right = ELEMENT_MOD_KSL
    },
    [ELEMENT_MOD_ENVELOPE_TYPE] = {
        .type = TYPE_LED, .max = 1, .x = 7, .y = 8, .height = 2,
        .up   = ELEMENT_MOD_KSR,            .down  = ELEMENT_CAR_KSR,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_AM
    },
    [ELEMENT_MOD_AM] = {
        .type = TYPE_LED, .max = 1, .x = 10, .y = 8, .height = 2,
        .up   = ELEMENT_MOD_VIBRATO,        .down  = ELEMENT_CAR_VIBRATO,
        .left = ELEMENT_MOD_ENVELOPE_TYPE,  .right = ELEMENT_MOD_WAVEFORM
    },
    [ELEMENT_MOD_KSL] = {
        .type = TYPE_VALUE, .max = 3, .x = 16, .y = 6, .height = 3,
        .up   = ELEMENT_SUSTAIN,            .down  = ELEMENT_CAR_KSL,
        .left = ELEMENT_MOD_WAVEFORM,       .right = ELEMENT_MOD_ATTACK_RATE
    },
    [ELEMENT_MOD_ATTACK_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 19, .y = 6, .height = 3,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_ATTACK_RATE,
        .left = ELEMENT_MOD_KSL,            .right = ELEMENT_MOD_DECAY_RATE
    },
    [ELEMENT_MOD_DECAY_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 22, .y = 6, .height = 3,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_DECAY_RATE,
        .left = ELEMENT_MOD_ATTACK_RATE,    .right = ELEMENT_MOD_SUSTAIN_LEVEL
    },
    [ELEMENT_MOD_SUSTAIN_LEVEL] = {
        .type = TYPE_VALUE, .max = 15, .x = 25, .y = 6, .height = 3,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_SUSTAIN_LEVEL,
        .left = ELEMENT_MOD_DECAY_RATE,     .right = ELEMENT_MOD_RELEASE_RATE
    },
    [ELEMENT_MOD_RELEASE_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 28, .y = 6, .height = 3,
        .up   = ELEMENT_TOTAL_LEVEL,        .down  = ELEMENT_CAR_RELEASE_RATE,
        .left = ELEMENT_MOD_SUSTAIN_LEVEL,  .right = ELEMENT_MOD_RELEASE_RATE
    },
    [ELEMENT_CAR_MULTI] = {
        .type = TYPE_VALUE, .max = 15, .x = 4, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_MULTI,          .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_KSR
    },
    [ELEMENT_CAR_KSR] = {
        .type = TYPE_LED, .max = 1, .x = 7, .y = 11, .height = 2,
        .up   = ELEMENT_MOD_ENVELOPE_TYPE,  .down  = ELEMENT_CAR_ENVELOPE_TYPE,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_VIBRATO
    },
    [ELEMENT_CAR_VIBRATO] = {
        .type = TYPE_LED, .max = 1, .x = 10, .y = 11, .height = 2,
        .up   = ELEMENT_MOD_AM,             .down  = ELEMENT_CAR_AM,
        .left = ELEMENT_CAR_KSR,            .right = ELEMENT_CAR_WAVEFORM
    },
    [ELEMENT_CAR_WAVEFORM] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_WAVEFORM,       .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_VIBRATO,        .right = ELEMENT_CAR_KSL
    },
    [ELEMENT_CAR_ENVELOPE_TYPE] = {
        .type = TYPE_LED, .max = 1, .x = 7, .y = 13, .height = 2,
        .up   = ELEMENT_CAR_KSR,            .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_AM
    },
    [ELEMENT_CAR_AM] = {
        .type = TYPE_LED, .max = 1, .x = 10, .y = 13, .height = 2,
        .up   = ELEMENT_CAR_VIBRATO,        .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_ENVELOPE_TYPE, .right = ELEMENT_CAR_WAVEFORM
    },
    [ELEMENT_CAR_KSL] = {
        .type = TYPE_VALUE, .max = 3, .x = 16, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_KSL,            .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_WAVEFORM,       .right = ELEMENT_CAR_ATTACK_RATE
    },
    [ELEMENT_CAR_ATTACK_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 19, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_ATTACK_RATE,    .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_KSL,            .right = ELEMENT_CAR_DECAY_RATE
    },
    [ELEMENT_CAR_DECAY_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 22, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_DECAY_RATE,     .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_ATTACK_RATE,    .right = ELEMENT_CAR_SUSTAIN_LEVEL
    },
    [ELEMENT_CAR_SUSTAIN_LEVEL] = {
        .type = TYPE_VALUE, .max = 15, .x = 25, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_SUSTAIN_LEVEL, .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_DECAY_RATE,     .right = ELEMENT_CAR_RELEASE_RATE
    },
    [ELEMENT_CAR_RELEASE_RATE] = {
        .type = TYPE_VALUE, .max = 15, .x = 28, .y = 11, .height = 3,
        .up   = ELEMENT_MOD_RELEASE_RATE,   .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_SUSTAIN_LEVEL,  .right = ELEMENT_CAR_RELEASE_RATE
    },
    [ELEMENT_KEYBOARD] = {
        .down = ELEMENT_KEYBOARD,
        .left = ELEMENT_KEYBOARD,           .right = ELEMENT_KEYBOARD
    }
};


/*
 * Update the cursor position.
 */
void cursor_update (uint8_t x, uint8_t y, uint8_t height)
{
    x = (x << 3) - 8;
    y = (y << 3) - 4;

    /* Clear any previous sprites */
    SMS_initSprites ();

    SMS_addSprite (x,      y,      PATTERN_CURSOR + 0);
    SMS_addSprite (x +  8, y,      PATTERN_CURSOR + 1);
    SMS_addSprite (x + 16, y,      PATTERN_CURSOR + 2);

    for (uint8_t i = 1; i < height; i++)
    {
        SMS_addSprite (x,      y + (i << 3), PATTERN_CURSOR + 3);
        SMS_addSprite (x + 16, y + (i << 3), PATTERN_CURSOR + 5);
    }

    SMS_addSprite (x,      y + (height << 3), PATTERN_CURSOR + 6);
    SMS_addSprite (x +  8, y + (height << 3), PATTERN_CURSOR + 7);
    SMS_addSprite (x + 16, y + (height << 3), PATTERN_CURSOR + 8);

    SMS_copySpritestoSAT ();
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
    SMS_useFirstHalfTilesforSprites (true);
    SMS_initSprites ();
    SMS_copySpritestoSAT ();
    SMS_setBGScrollX (0xfc);

    draw_reset ();
    draw_labels ();
    draw_keyboard ();

    /* Initial draw of GUI elements */
    for (uint8_t i = 0; i < ELEMENT_KEYBOARD; i++)
    {
        gui_element_t *element = &main_gui [i];

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, 0);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, false);
        }
    }

    element_id_t current_element = ELEMENT_INSTRUMENT;
    uint8_t keyboard_key = 0;

    cursor_update (main_gui [current_element].x,
                   main_gui [current_element].y,
                   main_gui [current_element].height);

    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
        uint16_t keys_pressed = SMS_getKeysPressed ();

        switch (keys_pressed)
        {
            case PORT_A_KEY_UP:
                if (current_element == ELEMENT_KEYBOARD)
                {
                    /* TODO: This keyboard logic should be moved out of main. */
                    draw_keyboard_update (keyboard_key, false);
                    if      (keyboard_key < 5 ) current_element = ELEMENT_CAR_MULTI;
                    else if (keyboard_key < 8 ) current_element = ELEMENT_CAR_ENVELOPE_TYPE;
                    else if (keyboard_key < 11) current_element = ELEMENT_CAR_AM;
                    else if (keyboard_key < 14) current_element = ELEMENT_CAR_WAVEFORM;
                    else if (keyboard_key < 17) current_element = ELEMENT_CAR_KSL;
                    else if (keyboard_key < 20) current_element = ELEMENT_CAR_ATTACK_RATE;
                    else if (keyboard_key < 23) current_element = ELEMENT_CAR_DECAY_RATE;
                    else if (keyboard_key < 26) current_element = ELEMENT_CAR_SUSTAIN_LEVEL;
                    else                        current_element = ELEMENT_CAR_RELEASE_RATE;
                }
                else
                {
                    current_element = main_gui [current_element].up;
                }
                break;
            case PORT_A_KEY_DOWN:
                if (current_element != ELEMENT_KEYBOARD)
                {
                    if (main_gui [current_element].down == ELEMENT_KEYBOARD)
                    {
                        /* Hide the cursor */
                        SMS_initSprites ();
                        SMS_copySpritestoSAT ();

                        /* Select a key */
                        keyboard_key = main_gui [current_element].x - 2;
                        draw_keyboard_update (keyboard_key, true);
                    }
                    current_element = main_gui [current_element].down;
                }
                break;
            case PORT_A_KEY_LEFT:
                if (current_element == ELEMENT_KEYBOARD)
                {
                    if (keyboard_key > 0)
                    {
                        draw_keyboard_update (keyboard_key, false);
                        keyboard_key--;
                        draw_keyboard_update (keyboard_key, true);
                    }
                }
                else
                {
                    current_element = main_gui [current_element].left;
                }
                break;
            case PORT_A_KEY_RIGHT:
                if (current_element == ELEMENT_KEYBOARD)
                {
                    if (keyboard_key < 28)
                    {
                        draw_keyboard_update (keyboard_key, false);
                        keyboard_key++;
                        draw_keyboard_update (keyboard_key, true);
                    }
                }
                else
                {
                    current_element = main_gui [current_element].right;
                }
                break;
            default:
                break;
        }

        /* TODO: Only update when a change occurred */
        if (current_element != ELEMENT_KEYBOARD)
        {
            cursor_update (main_gui [current_element].x,
                           main_gui [current_element].y,
                           main_gui [current_element].height);
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
