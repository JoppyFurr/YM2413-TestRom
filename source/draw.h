/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

/* Initialise the keyboard display. */
void draw_keyboard (void);

/* Update the display of a key to be either active or inactive. */
void draw_keyboard_update (uint8_t key, bool active);

/* Fill the name table with tile-zero. */
void draw_reset (void);
