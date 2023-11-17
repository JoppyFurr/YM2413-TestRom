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
#include "gui_elements.h"
#include "draw.h"


typedef struct gui_state_s {

    element_id_t current_element;
    uint8_t keyboard_key;

    bool keyboard_update;
    bool cursor_update;

    uint16_t element_values [ELEMENT_COUNT];

} gui_state_t;


/*
 * Update the cursor position.
 */
static void cursor_update (uint8_t x, uint8_t y, uint8_t height)
{
    /* Clear any previous sprites */
    SMS_initSprites ();

    /* A height of zero indicates that we do not want to draw the cursor */
    if (height > 0)
    {
        x = (x << 3) - 8;
        y = (y << 3) - 4;

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
    }

    SMS_copySpritestoSAT ();
}


/*
 * Move the cursor to select a different GUI element.
 */
static void element_navigate (gui_state_t *gui_state, uint16_t key_pressed)
{
    if (gui_state->current_element == ELEMENT_KEYBOARD)
    {
        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                if      (gui_state->keyboard_key < 6 ) gui_state->current_element = ELEMENT_CAR_MULTI;
                else if (gui_state->keyboard_key < 9 ) gui_state->current_element = ELEMENT_CAR_ENVELOPE_TYPE;
                else if (gui_state->keyboard_key < 12) gui_state->current_element = ELEMENT_CAR_AM;
                else if (gui_state->keyboard_key < 15) gui_state->current_element = ELEMENT_CAR_WAVEFORM;
                else if (gui_state->keyboard_key < 18) gui_state->current_element = ELEMENT_CAR_KSL;
                else if (gui_state->keyboard_key < 21) gui_state->current_element = ELEMENT_CAR_ATTACK_RATE;
                else if (gui_state->keyboard_key < 24) gui_state->current_element = ELEMENT_CAR_DECAY_RATE;
                else if (gui_state->keyboard_key < 27) gui_state->current_element = ELEMENT_CAR_SUSTAIN_LEVEL;
                else                        gui_state->current_element = ELEMENT_CAR_RELEASE_RATE;
                gui_state->cursor_update = true;
                gui_state->keyboard_key = 0;
                gui_state->keyboard_update = true;
                break;
            case PORT_A_KEY_LEFT:
                if (gui_state->keyboard_key > 1)
                {
                    gui_state->keyboard_key--;
                    gui_state->keyboard_update = true;
                }
                break;
            case PORT_A_KEY_RIGHT:
                if (gui_state->keyboard_key < 29)
                {
                    gui_state->keyboard_key++;
                    gui_state->keyboard_update = true;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        const gui_element_t *element = &main_gui [gui_state->current_element];

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                gui_state->current_element = element->up;
                break;

            case PORT_A_KEY_DOWN:
                if (element->down == ELEMENT_KEYBOARD)
                {
                    /* Hide the cursor */
                    SMS_initSprites ();
                    SMS_copySpritestoSAT ();

                    /* Select a key */
                    gui_state->keyboard_key = element->x - 1;
                    gui_state->keyboard_update = true;
                }
                gui_state->current_element = element->down;
                break;

            case PORT_A_KEY_LEFT:
                gui_state->current_element = element->left;
                break;

            case PORT_A_KEY_RIGHT:
                gui_state->current_element = element->right;
                break;

            default:
                break;
        }
        gui_state->cursor_update = true;
    }
}
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

    gui_state_t gui_state = { .current_element = ELEMENT_INSTRUMENT };

    draw_reset ();
    draw_labels ();
    draw_keyboard ();

    /* Initial draw of GUI elements */
    for (uint8_t i = 0; i < ELEMENT_KEYBOARD; i++)
    {
        const gui_element_t *element = &main_gui [i];

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, 0);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, false);
        }
    }

    cursor_update (main_gui [gui_state.current_element].x,
                   main_gui [gui_state.current_element].y,
                   main_gui [gui_state.current_element].height);

    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
        uint16_t key_pressed = SMS_getKeysPressed ();

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
            case PORT_A_KEY_DOWN:
            case PORT_A_KEY_LEFT:
            case PORT_A_KEY_RIGHT:
                element_navigate (&gui_state, key_pressed);
                break;

            default:
                break;
        }

        if (gui_state.cursor_update)
        {
            cursor_update (main_gui [gui_state.current_element].x,
                           main_gui [gui_state.current_element].y,
                           main_gui [gui_state.current_element].height);
            gui_state.cursor_update = false;
        }

        if (gui_state.keyboard_update)
        {
            static uint8_t previous_key = 0;

            if (previous_key > 0)
            {
                draw_keyboard_update (previous_key - 1, false);
            }

            if (gui_state.keyboard_key > 0)
            {
                draw_keyboard_update (gui_state.keyboard_key - 1, true);
            }

            previous_key = gui_state.keyboard_key;
            gui_state.keyboard_update = false;
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
