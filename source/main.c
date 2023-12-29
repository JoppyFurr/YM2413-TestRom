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
#include "register.h"
#include "gui_elements.h"
#include "draw.h"


typedef struct gui_state_s {

    element_id_t current_element;
    uint8_t keyboard_key;

    bool cursor_update;
    bool keyboard_update;
    bool element_update;

    uint16_t element_values [ELEMENT_COUNT];

} gui_state_t;

typedef struct note_s {
    uint16_t fnum;
    uint8_t block;
} note_t;

static const uint16_t register_defaults [ELEMENT_COUNT] = {
    [ELEMENT_INSTRUMENT] = 0,
    [ELEMENT_VOLUME] = 0,
    [ELEMENT_SUSTAIN] = 0,
    [ELEMENT_FEEDBACK] = 0,
    [ELEMENT_TOTAL_LEVEL] = 40,
    [ELEMENT_MOD_MULTI] = 1,
    [ELEMENT_MOD_KSR] = 0,
    [ELEMENT_MOD_VIBRATO] = 0,
    [ELEMENT_MOD_WAVEFORM] = 0,
    [ELEMENT_MOD_ENVELOPE_TYPE] = 0,
    [ELEMENT_MOD_AM] = 0,
    [ELEMENT_MOD_KSL] = 0,
    [ELEMENT_MOD_ATTACK_RATE] = 15,
    [ELEMENT_MOD_DECAY_RATE] = 0,
    [ELEMENT_MOD_SUSTAIN_LEVEL] = 0,
    [ELEMENT_MOD_RELEASE_RATE] = 0,
    [ELEMENT_CAR_MULTI] = 1,
    [ELEMENT_CAR_KSR] = 0,
    [ELEMENT_CAR_VIBRATO] = 0,
    [ELEMENT_CAR_WAVEFORM] = 0,
    [ELEMENT_CAR_ENVELOPE_TYPE] = 0,
    [ELEMENT_CAR_AM] = 0,
    [ELEMENT_CAR_KSL] = 0,
    [ELEMENT_CAR_ATTACK_RATE] = 15,
    [ELEMENT_CAR_DECAY_RATE] = 0,
    [ELEMENT_CAR_SUSTAIN_LEVEL] = 0,
    [ELEMENT_CAR_RELEASE_RATE] = 0
};

note_t notes [29] = {
    {345, 2}, {365, 2}, {387, 2}, {410, 2}, {435, 2}, {460, 2}, {488, 2}, {258, 3},
    {274, 3}, {290, 3}, {307, 3}, {326, 3}, {345, 3}, {365, 3}, {387, 3}, {410, 3},
    {435, 3}, {460, 3}, {488, 3}, {258, 4}, {274, 4}, {290, 4}, {307, 4}, {326, 4},
    {345, 4}, {365, 4}, {387, 4}, {410, 4}, {435, 4}
};


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
 * Disable the display of custom instrument parameters.
 */
static void custom_instrument_hide (gui_state_t *state, bool hide)
{
    if (hide)
    {
        for (uint8_t i = ELEMENT_FEEDBACK; i < ELEMENT_KEYBOARD; i++)
        {
            const gui_element_t *element = &main_gui [i];

            if (element->type == TYPE_VALUE)
            {
                draw_value_hidden (element->x, element->y);
            }
            else if (element->type == TYPE_LED)
            {
                draw_led (element->x, element->y, false);
            }
        }
    }
    else
    {
        for (uint8_t i = ELEMENT_FEEDBACK; i < ELEMENT_KEYBOARD; i++)
        {
            const gui_element_t *element = &main_gui [i];
            uint16_t value = state->element_values [i];

            if (element->type == TYPE_VALUE)
            {
                draw_value (element->x, element->y, value);
            }
            else if (element->type == TYPE_LED)
            {
                draw_led (element->x, element->y, value);
            }
        }
    }
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


/*
 * Update the value of an parameter when a button is pressed.
 */
static void element_update (gui_state_t *state, uint16_t key_pressed)
{
    const gui_element_t *element = &main_gui [state->current_element];
    uint16_t *value = &state->element_values [state->current_element];

    /* Ignore custom instrument parameters when the custom instrument isn't selected */
    if (state->element_values [ELEMENT_INSTRUMENT] != 0 &&
        state->current_element >= ELEMENT_FEEDBACK && state->current_element <= ELEMENT_CAR_RELEASE_RATE)
    {
        return;
    }

    if (element->type == TYPE_VALUE)
    {
        if (key_pressed == PORT_A_KEY_1 && *value > 0)
        {
            *value -= 1;
            state->element_update = true;
        }
        else if (key_pressed == PORT_A_KEY_2 && *value < element->max)
        {
            *value += 1;
            state->element_update = true;
        }
    }
    else if (element->type == TYPE_LED)
    {
        *value ^= 0x0001;
        state->element_update = true;
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
    draw_name (0);

    /* Initialise GUI elements and register defaults */
    for (uint8_t i = 0; i < ELEMENT_KEYBOARD; i++)
    {
        const gui_element_t *element = &main_gui [i];
        uint16_t value = register_defaults [i];
        gui_state.element_values [i] = value;

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, value);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, value);
        }

        if (element->callback)
        {
            element->callback (value);
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
        uint16_t key_released = SMS_getKeysReleased ();

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
            case PORT_A_KEY_DOWN:
            case PORT_A_KEY_LEFT:
            case PORT_A_KEY_RIGHT:
                element_navigate (&gui_state, key_pressed);
                break;

            case PORT_A_KEY_1:
            case PORT_A_KEY_2:
                element_update (&gui_state, key_pressed);
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

            if (gui_state.current_element == ELEMENT_KEYBOARD)
            {
                draw_keyboard_update (gui_state.keyboard_key - 1, true);

                note_t *note = &notes [gui_state.keyboard_key - 1];
                register_write_fnum_block (note->fnum, note->block);
            }
            else
            {
                /* Treat leaving the keyboard as letting go of the key */
                register_write_key_on (0);
            }


            previous_key = gui_state.keyboard_key;
            gui_state.keyboard_update = false;
        }

        if (gui_state.element_update)
        {
            const gui_element_t *element = &main_gui [gui_state.current_element];
            uint16_t value = gui_state.element_values [gui_state.current_element];

            if (element->type == TYPE_VALUE)
            {
                draw_value (element->x, element->y, value);
            }
            else if (element->type == TYPE_LED)
            {
                draw_led (element->x, element->y, value);
            }

            if (element->callback)
            {
                element->callback (value);
            }

            /* Dim custom instrument settings when the custom instrument is not in use */
            if (gui_state.current_element == ELEMENT_INSTRUMENT)
            {
                custom_instrument_hide (&gui_state, value > 0);
                draw_name (value);
            }

            gui_state.element_update = false;
        }

        if (gui_state.current_element == ELEMENT_KEYBOARD)
        {
            if (key_pressed == PORT_A_KEY_1)
            {
                register_write_key_on (1);
            }
            else if (key_released == PORT_A_KEY_1)
            {
                register_write_key_on (0);
            }
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
