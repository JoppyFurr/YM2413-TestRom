/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#define PORT_A_DPAD_MASK    (PORT_A_KEY_UP | PORT_A_KEY_DOWN | PORT_A_KEY_LEFT | PORT_A_KEY_RIGHT)
#define PORT_A_KEY_MASK     (PORT_A_KEY_1 | PORT_A_KEY_2)

#include "../tile_data/palette.h"
#include "../tile_data/pattern.h"
#include "../tile_data/pattern_index.h"
#include "register.h"
#include "gui_elements.h"
#include "draw.h"


typedef struct gui_state_s {

    const gui_element_t *gui;
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
    [ELEMENT_CAR_RELEASE_RATE] = 0,
    [ELEMENT_CH6_SUSTAIN] = 0,
    [ELEMENT_CH6_BLOCK] = 2,
    [ELEMENT_CH6_FNUM] = 288,
    [ELEMENT_CH7_SUSTAIN] = 0,
    [ELEMENT_CH7_BLOCK] = 2,
    [ELEMENT_CH7_FNUM] = 336,
    [ELEMENT_CH8_SUSTAIN] = 0,
    [ELEMENT_CH8_BLOCK] = 0,
    [ELEMENT_CH8_FNUM] = 448,
    [ELEMENT_BD_VOLUME] = 0,
    [ELEMENT_BD_BUTTON] = 0,
    [ELEMENT_HH_VOLUME] = 0,
    [ELEMENT_HH_BUTTON] = 0,
    [ELEMENT_SD_VOLUME] = 0,
    [ELEMENT_SD_BUTTON] = 0,
    [ELEMENT_TT_VOLUME] = 0,
    [ELEMENT_TT_BUTTON] = 0,
    [ELEMENT_TC_VOLUME] = 0,
    [ELEMENT_TC_BUTTON] = 0
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
static void cursor_update (uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    /* Clear any previous sprites */
    SMS_initSprites ();

    /* A height of zero indicates that we do not want to draw the cursor */
    if (height > 0)
    {
        x = (x << 3) - 8;
        y = (y << 3) - 4;

        /* Top */
        SMS_addSprite (x, y, PATTERN_CURSOR + 0);
        for (uint8_t i = 1; i < width; i++)
        {
            SMS_addSprite (x + (i << 3), y, PATTERN_CURSOR + 1);
        }
        SMS_addSprite (x + (width << 3), y, PATTERN_CURSOR + 2);

        /* Sides */
        for (uint8_t i = 1; i < height; i++)
        {
            SMS_addSprite (x,                y + (i << 3), PATTERN_CURSOR + 3);
            SMS_addSprite (x + (width << 3), y + (i << 3), PATTERN_CURSOR + 5);
        }

        /* Bottom */
        SMS_addSprite (x, y + (height << 3), PATTERN_CURSOR + 6);
        for (uint8_t i = 1; i < width; i++)
        {
            SMS_addSprite (x + (i << 3), y + (height << 3), PATTERN_CURSOR + 7);
        }
        SMS_addSprite (x + (width << 3), y + (height << 3), PATTERN_CURSOR + 8);
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
            const gui_element_t *element = &state->gui [i];

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
            const gui_element_t *element = &state->gui [i];
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
 * Push the in-ram copy of an element's value to the GUI and run its callback
 * to push the value to the ym2413.
 */
static void element_update (const gui_element_t *element, uint16_t value)
{
    if (element->type == TYPE_VALUE)
    {
        draw_value (element->x, element->y, value);
    }
    else if (element->type == TYPE_VALUE_WIDE)
    {
        draw_value_wide (element->x, element->y, value);
    }
    else if (element->type == TYPE_LED)
    {
        draw_led (element->x, element->y, value);
    }
    else if (element->type == TYPE_BUTTON)
    {
        draw_button (element->x, element->y, value);
    }

    if (element->callback)
    {
        element->callback (value);
    }
}


/*
 * Move the cursor to select a different GUI element.
 */
static void element_navigate (gui_state_t *state, uint16_t key_pressed)
{
    if (state->current_element == ELEMENT_KEYBOARD)
    {
        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                if      (state->keyboard_key < 6 ) state->current_element = ELEMENT_CAR_MULTI;
                else if (state->keyboard_key < 9 ) state->current_element = ELEMENT_CAR_ENVELOPE_TYPE;
                else if (state->keyboard_key < 12) state->current_element = ELEMENT_CAR_AM;
                else if (state->keyboard_key < 15) state->current_element = ELEMENT_CAR_WAVEFORM;
                else if (state->keyboard_key < 18) state->current_element = ELEMENT_CAR_KSL;
                else if (state->keyboard_key < 21) state->current_element = ELEMENT_CAR_ATTACK_RATE;
                else if (state->keyboard_key < 24) state->current_element = ELEMENT_CAR_DECAY_RATE;
                else if (state->keyboard_key < 27) state->current_element = ELEMENT_CAR_SUSTAIN_LEVEL;
                else                               state->current_element = ELEMENT_CAR_RELEASE_RATE;
                state->cursor_update = true;
                state->keyboard_key = 0;
                state->keyboard_update = true;
                break;
            case PORT_A_KEY_LEFT:
                if (state->keyboard_key > 1)
                {
                    state->keyboard_key--;
                    state->keyboard_update = true;
                }
                break;
            case PORT_A_KEY_RIGHT:
                if (state->keyboard_key < 29)
                {
                    state->keyboard_key++;
                    state->keyboard_update = true;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        uint8_t element_was = state->current_element;
        const gui_element_t *element = &state->gui [state->current_element];

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                state->current_element = element->up;
                break;

            case PORT_A_KEY_DOWN:
                if (element->down == ELEMENT_KEYBOARD)
                {
                    /* Hide the cursor */
                    SMS_initSprites ();
                    SMS_copySpritestoSAT ();

                    /* Select a key */
                    state->keyboard_key = element->x - 1;
                    state->keyboard_update = true;
                }
                state->current_element = element->down;
                break;

            case PORT_A_KEY_LEFT:
                state->current_element = element->left;
                break;

            case PORT_A_KEY_RIGHT:
                state->current_element = element->right;
                break;

            default:
                break;
        }

        /* "BUTTON" elements turn off when unselected.
         * Note that our pointer, 'element', still points at the previous element */
        if (state->current_element != element_was)
        {
            if (element->type == TYPE_BUTTON)
            {
                state->element_values [element_was] = 0;
                element_update (element, 0);
            }
        }

        state->cursor_update = true;
    }
}


/*
 * Update the in-ram value of an element when a button is pressed or released.
 */
static void element_input (gui_state_t *state, uint16_t key_pressed, int16_t key_released)
{
    const gui_element_t *element = &state->gui [state->current_element];
    uint16_t *value = &state->element_values [state->current_element];

    /* Ignore custom instrument parameters when the custom instrument isn't selected */
    if (state->element_values [ELEMENT_INSTRUMENT] != 0 &&
        state->current_element >= ELEMENT_FEEDBACK && state->current_element <= ELEMENT_CAR_RELEASE_RATE)
    {
        return;
    }

    if (element->type == TYPE_VALUE || element->type == TYPE_VALUE_WIDE)
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
        if (key_pressed)
        {
            *value ^= 0x0001;
            state->element_update = true;
        }
    }
    else if (element->type == TYPE_BUTTON)
    {
        if (key_pressed)
        {
            *value = 1;
        }
        else if (key_released)
        {
            *value = 0;
        }
        state->element_update = true;
    }
}


/*
 * Configure the GUI for melody mode.
 */
static void melody_mode (gui_state_t *state)
{
    state->gui = melody_gui;
    draw_tabs (false);

    /* Clear rhythm area */
    draw_reset (2, 23);

    draw_melody_labels ();

    /* Draw the GUI elements with their current values */
    for (uint8_t i = ELEMENT_INSTRUMENT; i <= ELEMENT_SUSTAIN; i++)
    {
        const gui_element_t *element = &state->gui [i];
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
    custom_instrument_hide (state, state->element_values [ELEMENT_INSTRUMENT] > 0);

    draw_keyboard ();
    draw_instrument_name (0);
}


/*
 * Configure the GUI for rhythm mode.
 */
void rhythm_mode (gui_state_t *state)
{
    state->gui = rhythm_gui;
    draw_tabs (true);

    /* Clear melody area */
    draw_reset (2, 23);

    draw_rhythm_labels ();

    /* Draw the GUI elements with their current values */
    for (uint8_t i = ELEMENT_CH6_SUSTAIN; i <= ELEMENT_TC_BUTTON; i++)
    {
        const gui_element_t *element = &state->gui [i];
        uint16_t value = state->element_values [i];

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, value);
        }
        else if (element->type == TYPE_VALUE_WIDE)
        {
            draw_value_wide (element->x, element->y, value);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, value);
        }
        else if (element->type == TYPE_BUTTON)
        {
            draw_button (element->x, element->y, value);
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

    gui_state_t gui_state = { .gui = melody_gui, .current_element = ELEMENT_INSTRUMENT };

    draw_reset (0, 24);
    draw_title ();
    draw_footer ();

    /* Put the chip into rhythm mode */
    register_write_rhythm_mode (1);

    /* Initialise register defaults */
    for (uint8_t i = ELEMENT_INSTRUMENT; i < ELEMENT_TC_BUTTON; i++)
    {
        const gui_element_t *element;
        uint16_t value = register_defaults [i];

        if (i <= ELEMENT_KEYBOARD)
        {
            element = &melody_gui [i];
        }
        else
        {
            element = &rhythm_gui [i];
        }
        gui_state.element_values [i] = value;

        if (element->callback)
        {
            element->callback (value);
        }
    }

    melody_mode (&gui_state);

    cursor_update (gui_state.gui [gui_state.current_element].x,
                   gui_state.gui [gui_state.current_element].y,
                   gui_state.gui [gui_state.current_element].width,
                   gui_state.gui [gui_state.current_element].height);

    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
        uint16_t key_pressed = SMS_getKeysPressed ();
        uint16_t key_released = SMS_getKeysReleased ();

        /* Navigation */
        switch (key_pressed & PORT_A_DPAD_MASK)
        {
            case PORT_A_KEY_UP:
            case PORT_A_KEY_DOWN:
            case PORT_A_KEY_LEFT:
            case PORT_A_KEY_RIGHT:
                element_navigate (&gui_state, key_pressed);
                break;
        }

        /* Button input */
        switch ((key_pressed | key_released) & PORT_A_KEY_MASK)
        {
            case PORT_A_KEY_1:
            case PORT_A_KEY_2:
                element_input (&gui_state, key_pressed, key_released);
                break;

            default:
                break;
        }

        if (gui_state.cursor_update)
        {
            cursor_update (gui_state.gui [gui_state.current_element].x,
                           gui_state.gui [gui_state.current_element].y,
                           gui_state.gui [gui_state.current_element].width,
                           gui_state.gui [gui_state.current_element].height);
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
            const gui_element_t *element = &gui_state.gui [gui_state.current_element];
            uint16_t value = gui_state.element_values [gui_state.current_element];

            element_update (element, value);

            /* Dim custom instrument settings when the custom instrument is not in use */
            if (gui_state.current_element == ELEMENT_INSTRUMENT)
            {
                custom_instrument_hide (&gui_state, value > 0);
                draw_instrument_name (value);
            }

            gui_state.element_update = false;
        }

        /* Melody-specific actions */
        if (gui_state.gui == melody_gui)
        {
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

            if (gui_state.current_element == ELEMENT_RHYTHM_TAB && key_pressed == PORT_A_KEY_1)
            {
                rhythm_mode (&gui_state);
            }
        }

        /* Rhythm-specific actions */
        else if (gui_state.gui == rhythm_gui)
        {
            if (gui_state.current_element == ELEMENT_MELODY_TAB && key_pressed == PORT_A_KEY_1)
            {
                melody_mode (&gui_state);
            }
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
