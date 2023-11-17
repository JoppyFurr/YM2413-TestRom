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


note_t notes [16] [29] = {

    /* Multiplication Factor = 0.5 */ {
    {345, 3}, {365, 3}, {387, 3}, {410, 3}, {435, 3}, {460, 3}, {488, 3}, {258, 4},
    {274, 4}, {290, 4}, {307, 4}, {326, 4}, {345, 4}, {365, 4}, {387, 4}, {410, 4},
    {435, 4}, {460, 4}, {488, 4}, {258, 5}, {274, 5}, {290, 5}, {307, 5}, {326, 5},
    {345, 5}, {365, 5}, {387, 5}, {410, 5}, {435, 5} },

    /* Multiplication factor = 1 */ {
    {345, 2}, {365, 2}, {387, 2}, {410, 2}, {435, 2}, {460, 2}, {488, 2}, {258, 3},
    {274, 3}, {290, 3}, {307, 3}, {326, 3}, {345, 3}, {365, 3}, {387, 3}, {410, 3},
    {435, 3}, {460, 3}, {488, 3}, {258, 4}, {274, 4}, {290, 4}, {307, 4}, {326, 4},
    {345, 4}, {365, 4}, {387, 4}, {410, 4}, {435, 4} },

    /* Multiplication Factor = 2 */ {
    {345, 1}, {365, 1}, {387, 1}, {410, 1}, {435, 1}, {460, 1}, {488, 1}, {258, 2},
    {274, 2}, {290, 2}, {307, 2}, {326, 2}, {345, 2}, {365, 2}, {387, 2}, {410, 2},
    {435, 2}, {460, 2}, {488, 2}, {258, 3}, {274, 3}, {290, 3}, {307, 3}, {326, 3},
    {345, 3}, {365, 3}, {387, 3}, {410, 3}, {435, 3} },

    /* Multiplication Factor = 3 */ {
    {460, 0}, {487, 0}, {258, 1}, {273, 1}, {290, 1}, {307, 1}, {325, 1}, {344, 1},
    {365, 1}, {387, 1}, {410, 1}, {434, 1}, {460, 1}, {487, 1}, {258, 2}, {273, 2},
    {290, 2}, {307, 2}, {325, 2}, {344, 2}, {365, 2}, {387, 2}, {410, 2}, {434, 2},
    {460, 2}, {487, 2}, {258, 3}, {273, 3}, {290, 3} },

    /* Multiplication Factor = 4 */ {
    {345, 0}, {365, 0}, {387, 0}, {410, 0}, {435, 0}, {460, 0}, {488, 0}, {258, 1},
    {274, 1}, {290, 1}, {307, 1}, {326, 1}, {345, 1}, {365, 1}, {387, 1}, {410, 1},
    {435, 1}, {460, 1}, {488, 1}, {258, 2}, {274, 2}, {290, 2}, {307, 2}, {326, 2},
    {345, 2}, {365, 2}, {387, 2}, {410, 2}, {435, 2} },

    /* Multiplication Factor = 5 */ {
    {276, 0}, {292, 0}, {310, 0}, {328, 0}, {348, 0}, {368, 0}, {390, 0}, {413, 0},
    {438, 0}, {464, 0}, {492, 0}, {260, 1}, {276, 1}, {292, 1}, {310, 1}, {328, 1},
    {348, 1}, {368, 1}, {390, 1}, {413, 1}, {438, 1}, {464, 1}, {492, 1}, {260, 2},
    {276, 2}, {292, 2}, {310, 2}, {328, 2}, {348, 2} },

    /* Multiplication Factor = 6 */ {
    {230, 0}, {244, 0}, {258, 0}, {273, 0}, {290, 0}, {307, 0}, {325, 0}, {344, 0},
    {365, 0}, {387, 0}, {410, 0}, {434, 0}, {460, 0}, {487, 0}, {258, 1}, {273, 1},
    {290, 1}, {307, 1}, {325, 1}, {344, 1}, {365, 1}, {387, 1}, {410, 1}, {434, 1},
    {460, 1}, {487, 1}, {258, 2}, {273, 2}, {290, 2} },

    /* Multiplication Factor = 7 */ {
    {197, 0}, {209, 0}, {221, 0}, {234, 0}, {248, 0}, {263, 0}, {279, 0}, {295, 0},
    {313, 0}, {331, 0}, {351, 0}, {372, 0}, {394, 0}, {418, 0}, {442, 0}, {469, 0},
    {497, 0}, {263, 1}, {279, 1}, {295, 1}, {313, 1}, {331, 1}, {351, 1}, {372, 1},
    {394, 1}, {418, 1}, {442, 1}, {469, 1}, {497, 1} },

    /* Multiplication Factor = 8 */ {
    {172, 0}, {183, 0}, {194, 0}, {205, 0}, {217, 0}, {230, 0}, {244, 0}, {258, 0},
    {274, 0}, {290, 0}, {307, 0}, {326, 0}, {345, 0}, {365, 0}, {387, 0}, {410, 0},
    {435, 0}, {460, 0}, {488, 0}, {258, 1}, {274, 1}, {290, 1}, {307, 1}, {326, 1},
    {345, 1}, {365, 1}, {387, 1}, {410, 1}, {435, 1} },

    /* Multiplication Factor = 9 */ {
    {153, 0}, {162, 0}, {172, 0}, {182, 0}, {193, 0}, {205, 0}, {217, 0}, {230, 0},
    {243, 0}, {258, 0}, {273, 0}, {289, 0}, {307, 0}, {325, 0}, {344, 0}, {365, 0},
    {386, 0}, {409, 0}, {434, 0}, {459, 0}, {487, 0}, {258, 1}, {273, 1}, {289, 1},
    {307, 1}, {325, 1}, {344, 1}, {365, 1}, {386, 1} },

    /* Multiplication Factor = 10 */
    /* Multiplication Factor = 10 */ {
    {138, 0}, {146, 0}, {155, 0}, {164, 0}, {174, 0}, {184, 0}, {195, 0}, {207, 0},
    {219, 0}, {232, 0}, {246, 0}, {260, 0}, {276, 0}, {292, 0}, {310, 0}, {328, 0},
    {348, 0}, {368, 0}, {390, 0}, {413, 0}, {438, 0}, {464, 0}, {492, 0}, {260, 1},
    {276, 1}, {292, 1}, {310, 1}, {328, 1}, {348, 1} }, {
    {138, 0}, {146, 0}, {155, 0}, {164, 0}, {174, 0}, {184, 0}, {195, 0}, {207, 0},
    {219, 0}, {232, 0}, {246, 0}, {260, 0}, {276, 0}, {292, 0}, {310, 0}, {328, 0},
    {348, 0}, {368, 0}, {390, 0}, {413, 0}, {438, 0}, {464, 0}, {492, 0}, {260, 1},
    {276, 1}, {292, 1}, {310, 1}, {328, 1}, {348, 1} },

    /* Multiplication Factor = 12 */
    /* Multiplication Factor = 12 */ {
    {115, 0}, {122, 0}, {129, 0}, {137, 0}, {145, 0}, {153, 0}, {163, 0}, {172, 0},
    {182, 0}, {193, 0}, {205, 0}, {217, 0}, {230, 0}, {244, 0}, {258, 0}, {273, 0},
    {290, 0}, {307, 0}, {325, 0}, {344, 0}, {365, 0}, {387, 0}, {410, 0}, {434, 0},
    {460, 0}, {487, 0}, {258, 1}, {273, 1}, {290, 1} }, {
    {115, 0}, {122, 0}, {129, 0}, {137, 0}, {145, 0}, {153, 0}, {163, 0}, {172, 0},
    {182, 0}, {193, 0}, {205, 0}, {217, 0}, {230, 0}, {244, 0}, {258, 0}, {273, 0},
    {290, 0}, {307, 0}, {325, 0}, {344, 0}, {365, 0}, {387, 0}, {410, 0}, {434, 0},
    {460, 0}, {487, 0}, {258, 1}, {273, 1}, {290, 1} },

    /* Multiplication Factor = 15 */
    /* Multiplication Factor = 15 */ {
    { 92, 0}, { 97, 0}, {103, 0}, {109, 0}, {116, 0}, {123, 0}, {130, 0}, {138, 0},
    {146, 0}, {155, 0}, {164, 0}, {174, 0}, {184, 0}, {195, 0}, {206, 0}, {219, 0},
    {232, 0}, {246, 0}, {260, 0}, {276, 0}, {292, 0}, {309, 0}, {328, 0}, {347, 0},
    {368, 0}, {390, 0}, {413, 0}, {437, 0}, {463, 0} }, {
    { 92, 0}, { 97, 0}, {103, 0}, {109, 0}, {116, 0}, {123, 0}, {130, 0}, {138, 0},
    {146, 0}, {155, 0}, {164, 0}, {174, 0}, {184, 0}, {195, 0}, {206, 0}, {219, 0},
    {232, 0}, {246, 0}, {260, 0}, {276, 0}, {292, 0}, {309, 0}, {328, 0}, {347, 0},
    {368, 0}, {390, 0}, {413, 0}, {437, 0}, {463, 0} }
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

            if (gui_state.keyboard_key > 0)
            {
                draw_keyboard_update (gui_state.keyboard_key - 1, true);
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

            gui_state.element_update = false;
        }

        if (gui_state.current_element == ELEMENT_KEYBOARD)
        {
            if (key_pressed == PORT_A_KEY_1)
            {
                uint8_t multi = 1;
                if (gui_state.element_values [ELEMENT_INSTRUMENT] == 0)
                {
                    multi = gui_state.element_values [ELEMENT_CAR_MULTI];
                }
                note_t *note = &notes [multi] [gui_state.keyboard_key - 1];
                register_write_fnum_block (note->fnum, note->block);
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
