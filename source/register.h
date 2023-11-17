/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

/* Update the f-num and block registers. */
void register_write_fnum_block (uint16_t fnum, uint8_t block);

/* Write the sustain register. */
void register_write_sustain (uint16_t value);

/* Write the key register. */
void register_write_key_on (uint16_t value);

/* Write the instrument register. */
void register_write_instrument (uint16_t value);

/* Write the volume register. */
void register_write_volume (uint16_t value);

