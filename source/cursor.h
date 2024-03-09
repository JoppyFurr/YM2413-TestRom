/*
 * YM2413 Test ROM
 * Joppy Furr 2024
 */

/* Set the new cursor position. */
void cursor_target (uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/* Run the cursor-slide animation */
void cursor_tick (void);
