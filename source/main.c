/*
 * YM2413 Test ROM
 * Joppy Furr 2024
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
#include "cursor.h"
#include "draw.h"
#include "register.h"
#include "gui_elements.h"


typedef struct gui_state_s {

    const gui_element_t *gui;
    element_id_t current_element;
    uint8_t keyboard_key;

    bool cursor_update;
    bool keyboard_update;
    bool element_update;

    uint16_t element_values [ELEMENT_COUNT];

} gui_state_t;

gui_state_t gui_state = { .gui = melody_gui, .current_element = ELEMENT_INSTRUMENT };

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
 * Disable the display of custom instrument parameters.
 */
static void custom_instrument_hide (bool hide)
{
    if (hide)
    {
        for (uint8_t i = ELEMENT_FEEDBACK; i < ELEMENT_KEYBOARD; i++)
        {
            const gui_element_t *element = &gui_state.gui [i];

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
            const gui_element_t *element = &gui_state.gui [i];
            uint16_t value = gui_state.element_values [i];

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
static void element_navigate (uint16_t key_pressed)
{
    if (gui_state.current_element == ELEMENT_KEYBOARD)
    {
        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                if      (gui_state.keyboard_key <  6) gui_state.current_element = ELEMENT_CAR_MULTI;
                else if (gui_state.keyboard_key <  9) gui_state.current_element = ELEMENT_CAR_ENVELOPE_TYPE;
                else if (gui_state.keyboard_key < 12) gui_state.current_element = ELEMENT_CAR_AM;
                else if (gui_state.keyboard_key < 15) gui_state.current_element = ELEMENT_CAR_WAVEFORM;
                else if (gui_state.keyboard_key < 18) gui_state.current_element = ELEMENT_CAR_KSL;
                else if (gui_state.keyboard_key < 21) gui_state.current_element = ELEMENT_CAR_ATTACK_RATE;
                else if (gui_state.keyboard_key < 24) gui_state.current_element = ELEMENT_CAR_DECAY_RATE;
                else if (gui_state.keyboard_key < 27) gui_state.current_element = ELEMENT_CAR_SUSTAIN_LEVEL;
                else                                  gui_state.current_element = ELEMENT_CAR_RELEASE_RATE;
                gui_state.cursor_update = true;
                gui_state.keyboard_key = 0;
                gui_state.keyboard_update = true;
                break;
            case PORT_A_KEY_LEFT:
                if (gui_state.keyboard_key > 1)
                {
                    gui_state.keyboard_key--;
                    gui_state.keyboard_update = true;
                }
                break;
            case PORT_A_KEY_RIGHT:
                if (gui_state.keyboard_key < 29)
                {
                    gui_state.keyboard_key++;
                    gui_state.keyboard_update = true;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        uint8_t element_was = gui_state.current_element;
        const gui_element_t *element = &gui_state.gui [gui_state.current_element];

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                gui_state.current_element = element->up;
                break;

            case PORT_A_KEY_UP | PORT_A_KEY_RIGHT:
                gui_state.current_element = gui_state.gui [element->right].up;
                break;

            case PORT_A_KEY_RIGHT:
                gui_state.current_element = element->right;
                break;

            case PORT_A_KEY_DOWN | PORT_A_KEY_RIGHT:
                gui_state.current_element = gui_state.gui [element->right].down;
                break;

            case PORT_A_KEY_DOWN:
                gui_state.current_element = element->down;
                break;

            case PORT_A_KEY_DOWN | PORT_A_KEY_LEFT:
                gui_state.current_element = gui_state.gui [element->left].down;
                break;

            case PORT_A_KEY_LEFT:
                gui_state.current_element = element->left;
                break;

            case PORT_A_KEY_UP | PORT_A_KEY_LEFT:
                gui_state.current_element = gui_state.gui [element->left].up;
                break;

            default:
                break;
        }

        if (gui_state.current_element != element_was)
        {
            /* Moving to the keyboard should hide the cursor */
            if (gui_state.current_element == ELEMENT_KEYBOARD)
            {
                /* Hide the cursor */
                SMS_initSprites ();
                SMS_copySpritestoSAT ();

                /* Select a key */
                gui_state.keyboard_key = element->x - 1;
                gui_state.keyboard_update = true;
            }

            /* "BUTTON" elements turn off when unselected. Note that
             * our pointer, 'element', still points at the previous element */
            if (element->type == TYPE_BUTTON)
            {
                gui_state.element_values [element_was] = 0;
                element_update (element, 0);
            }
        }

        gui_state.cursor_update = true;
    }
}


/*
 * Update the in-ram value of an element when a button is pressed or released.
 */
static void element_input (uint16_t key_pressed, int16_t key_released)
{
    const gui_element_t *element = &gui_state.gui [gui_state.current_element];
    uint16_t *value = &gui_state.element_values [gui_state.current_element];

    /* Ignore custom instrument parameters when the custom instrument isn't selected */
    if (gui_state.element_values [ELEMENT_INSTRUMENT] != 0 &&
        gui_state.current_element >= ELEMENT_FEEDBACK && gui_state.current_element <= ELEMENT_CAR_RELEASE_RATE)
    {
        return;
    }

    if (element->type == TYPE_VALUE || element->type == TYPE_VALUE_WIDE)
    {
        if (key_pressed == PORT_A_KEY_1 && *value > 0)
        {
            *value -= 1;
            gui_state.element_update = true;
        }
        else if (key_pressed == PORT_A_KEY_2 && *value < element->max)
        {
            *value += 1;
            gui_state.element_update = true;
        }
    }
    else if (element->type == TYPE_LED)
    {
        if (key_pressed)
        {
            *value ^= 0x0001;
            gui_state.element_update = true;
        }
    }
    else if (element->type == TYPE_BUTTON)
    {
        uint16_t key_status = SMS_getKeysStatus ();

        if (key_pressed)
        {
            *value = 1;
        }
        else if (key_released && (key_status & PORT_A_KEY_MASK) == 0)
        {
            *value = 0;
        }
        gui_state.element_update = true;
    }
}


/*
 * Configure the GUI for melody mode.
 */
static void melody_mode (void)
{
    gui_state.gui = melody_gui;
    draw_tabs (false);

    /* Clear rhythm area */
    draw_reset (2, 23);

    draw_melody_labels ();

    /* Draw the GUI elements with their current values */
    for (uint8_t i = ELEMENT_INSTRUMENT; i <= ELEMENT_SUSTAIN; i++)
    {
        const gui_element_t *element = &gui_state.gui [i];
        uint16_t value = gui_state.element_values [i];

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, value);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, value);
        }
    }
    custom_instrument_hide (gui_state.element_values [ELEMENT_INSTRUMENT] > 0);

    draw_keyboard ();
    draw_instrument_name (0);
}


/*
 * Configure the GUI for rhythm mode.
 */
void rhythm_mode (void)
{
    gui_state.gui = rhythm_gui;
    draw_tabs (true);

    /* Clear melody area */
    draw_reset (2, 23);

    draw_rhythm_labels ();

    /* Draw the GUI elements with their current values */
    for (uint8_t i = ELEMENT_CH6_SUSTAIN; i <= ELEMENT_TC_BUTTON; i++)
    {
        const gui_element_t *element = &gui_state.gui [i];
        uint16_t value = gui_state.element_values [i];

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
 * When changing element values, repeat the button press if held down.
 * Start repeating at 500 ms. Repeat 30 times per second.
 */
static void key_repeat (void)
{
    static uint8_t start_timer = 0;
    static uint8_t repeat_timer = 0;

    /* When the input changes, just reset the timer */
    if (SMS_getKeysPressed () || SMS_getKeysReleased ())
    {
        start_timer = 0;
        repeat_timer = 0;
        return;
    }

    /* Repeat should only affect value inputs */
    const gui_element_t *element = &gui_state.gui [gui_state.current_element];
    if (element->type != TYPE_VALUE && element->type != TYPE_VALUE_WIDE)
    {
        return;
    }

    /* If exactly one button is held down, run the timer */
    uint16_t key_status = SMS_getKeysStatus ();
    if (key_status == PORT_A_KEY_1 || key_status == PORT_A_KEY_2)
    {
        if (start_timer == 30) /* 30 frames for 500 ms start time */
        {
            if (++repeat_timer & 0x01) /* Every second frame for 30 repeats per second */
            {
                element_input (key_status, 0);
            }
        }
        else
        {
            start_timer++;
        }
    }
}


/*
 * Frame interrupt, used to colour-cycle the cursor.
 */
static void frame_interrupt (void)
{
    static uint8_t frame = 0;
    static uint8_t hilight_index = 3;
    frame++;

    /* Simple 3-frame palette cycle to animate cursor */
    if ((frame & 0x07) == 0)
    {
        static int band = 3;

        SMS_setSpritePaletteColor (band, 2); /* Dim the previously bright band */
        band = (band == 1) ? 3 : band - 1;
        SMS_setSpritePaletteColor (band, 23); /* Brighten the new bright band */
    }

    /* Animate the cursor slide */
    cursor_tick ();

    key_repeat ();
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
    SMS_setSpritePaletteColor (1, 2);   /* Cursor - Dark Red */
    SMS_setSpritePaletteColor (2, 2);   /* Cursor - Dark Red */
    SMS_setSpritePaletteColor (3, 23);  /* Cursor - Light Red */
    SMS_setSpritePaletteColor (4, 3);   /* Floating Digits - Pure Red */
    SMS_setBGPaletteColor (4, 58);      /* Selected key - Light Lavender */

    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_useFirstHalfTilesforSprites (true);
    SMS_initSprites ();
    SMS_copySpritestoSAT ();

    draw_reset (0, 24);
    draw_title ();
    draw_footer ();

    enable_fm ();

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

    melody_mode ();

    cursor_target (gui_state.gui [gui_state.current_element].cursor_x,
                   gui_state.gui [gui_state.current_element].cursor_y,
                   gui_state.gui [gui_state.current_element].cursor_w,
                   gui_state.gui [gui_state.current_element].cursor_h);

    SMS_setFrameInterruptHandler (frame_interrupt);
    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
        uint16_t key_pressed = SMS_getKeysPressed ();
        uint16_t key_released = SMS_getKeysReleased ();
        uint16_t key_status = SMS_getKeysStatus ();

        /* Navigation */
        if (key_pressed & PORT_A_DPAD_MASK)
        {
            element_navigate (key_pressed);
        }

        /* Button input */
        if ((key_pressed | key_released) & PORT_A_KEY_MASK)
        {
            element_input (key_pressed & PORT_A_KEY_MASK, key_released & PORT_A_KEY_MASK);
        }

        if (gui_state.cursor_update)
        {
            cursor_target (gui_state.gui [gui_state.current_element].cursor_x,
                           gui_state.gui [gui_state.current_element].cursor_y,
                           gui_state.gui [gui_state.current_element].cursor_w,
                           gui_state.gui [gui_state.current_element].cursor_h);
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
                if (register_stored_key_on ())
                {
                    draw_block_fnum (note->fnum, note->block);
                }

            }
            else
            {
                /* Treat leaving the keyboard as letting go of the key */
                register_write_key_on (0);
                SMS_setBGPaletteColor (4, 58); /* Light Lavender */
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
                custom_instrument_hide (value > 0);
                draw_instrument_name (value);
            }

            gui_state.element_update = false;
        }

        /* Melody-specific actions */
        if (gui_state.gui == melody_gui)
        {
            if (gui_state.current_element == ELEMENT_KEYBOARD)
            {
                if (key_pressed & PORT_A_KEY_MASK)
                {
                    register_write_key_on (1);
                    SMS_setBGPaletteColor (4, 37); /* Dark Lavender */
                    note_t *note = &notes [gui_state.keyboard_key - 1];
                    draw_block_fnum (note->fnum, note->block);
                }
                else if ((key_released & PORT_A_KEY_MASK) && (key_status & PORT_A_KEY_MASK) == 0)
                {
                    register_write_key_on (0);
                    SMS_setBGPaletteColor (4, 58); /* Light Lavender */

                    /* Hide Block, Fnum */
                    SMS_initSprites ();
                    SMS_copySpritestoSAT ();
                }
            }

            if (gui_state.current_element == ELEMENT_RHYTHM_TAB && key_pressed == PORT_A_KEY_1)
            {
                rhythm_mode ();
            }
        }

        /* Rhythm-specific actions */
        else if (gui_state.gui == rhythm_gui)
        {
            if (gui_state.current_element == ELEMENT_MELODY_TAB && key_pressed == PORT_A_KEY_1)
            {
                melody_mode ();
            }
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
