/*
 * YM2413 Test ROM
 * Joppy Furr 2024
 */

#include <stdint.h>

#include "SMSlib.h"

#include "cursor.h"
#include "../tile_data/pattern_index.h"

/* Target position for the newly selected item */
static uint8_t target_x = 0;
static uint8_t target_y = 0;
static uint8_t target_w = 0;
static uint8_t target_h = 0;

/* Position of the previously selected item */
static uint8_t previous_x = 0;
static uint8_t previous_y = 0;
static uint8_t previous_w = 0;
static uint8_t previous_h = 0;

/* Sliding cursor position used for drawing */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t cursor_w = 0;
static uint8_t cursor_h = 0;
static uint8_t cursor_frame = 0;

/* 16-bit working area for calculation animation */
static uint16_t cursor16_x = 0;
static uint16_t cursor16_y = 0;
static uint16_t cursor16_w = 0;
static uint16_t cursor16_h = 0;


/*
 * Draw the cursor to the screen.
 */
static void cursor_draw (void)
{
    /* Clear any previous sprites */
    SMS_initSprites ();

    /* The (x, y) coordinate refers to the area inside the cursor,
     * so subtract the cursor's width */
    uint8_t x = cursor_x - 3;
    uint8_t y = cursor_y - 3;

    /* Top corners */
    SMS_addSprite (x,                y, PATTERN_CURSOR + 0);
    SMS_addSprite (x + cursor_w - 2, y, PATTERN_CURSOR + 2);

    /* Bottom corners */
    SMS_addSprite (x,                y + cursor_h - 2, PATTERN_CURSOR + 6);
    SMS_addSprite (x + cursor_w - 2, y + cursor_h - 2, PATTERN_CURSOR + 8);

    /* Top & bottom edges */
    for (int16_t filler = x + 8; filler < (x + cursor_w - 2); filler += 8)
    {
        SMS_addSprite (filler, y,                PATTERN_CURSOR + 1);
        SMS_addSprite (filler, y + cursor_h - 2, PATTERN_CURSOR + 7);
    }

    /* Left & right edges */
    for (int16_t filler = y + 8; filler < (y + cursor_h - 2); filler += 8)
    {
        SMS_addSprite (x,                filler, PATTERN_CURSOR + 3);
        SMS_addSprite (x + cursor_w - 2, filler, PATTERN_CURSOR + 5);
    }

    SMS_copySpritestoSAT ();
}


/*
 * Hide the cursor.
 */
static void cursor_hide (void)
{
    SMS_initSprites ();
    SMS_copySpritestoSAT ();
}


/*
 * Run the cursor-slide animation.
 */
void cursor_tick (void)
{
    if (cursor_frame < 8)
    {
        cursor16_x += target_x - previous_x;
        cursor_x = cursor16_x >> 3;
        cursor16_y += target_y - previous_y;
        cursor_y = cursor16_y >> 3;
        cursor16_w += target_w - previous_w;
        cursor_w = cursor16_w >> 3;
        cursor16_h += target_h - previous_h;
        cursor_h = cursor16_h >> 3;
        cursor_frame++;
        cursor_draw ();
    }
}


/*
 * Set the new cursor position.
 */
void cursor_target (uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    previous_x = cursor_x;
    previous_y = cursor_y;
    previous_w = cursor_w;
    previous_h = cursor_h;

    target_x = x;
    target_y = y;
    target_w = w;
    target_h = h;

    /* No animation when hiding the cursor. */
    if (target_w == 0)
    {
        cursor_frame = 8;
        cursor_w = 0;
        cursor_hide ();
        return;
    }

    /* No animation when revealing the cursor. */
    if (previous_w == 0)
    {
        cursor_frame = 8;
        cursor_x = target_x;
        cursor_y = target_y;
        cursor_w = target_w;
        cursor_h = target_h;
        cursor_draw ();
        return;
    }

    /* Otherwise, begin the slide animation */
    cursor16_x = previous_x << 3;
    cursor16_y = previous_y << 3;
    cursor16_w = previous_w << 3;
    cursor16_h = previous_h << 3;
    cursor_frame = 0;
    cursor_tick ();
}
