/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

/* Write the modulator AM bit. */
void register_write_mod_am (uint16_t value);

/* Write the carrier AM bit. */
void register_write_car_am (uint16_t value);

/* Write the modulator vibrato bit. */
void register_write_mod_vibrato (uint16_t value);

/* Write the carrier vibrato bit. */
void register_write_car_vibrato (uint16_t value);

/* Write the modulator envelope-type bit. */
void register_write_mod_eg_type (uint16_t value);

/* Write the carrier envelope-type bit. */
void register_write_car_eg_type (uint16_t value);

/* Write the modulator key-scale rate bit. */
void register_write_mod_ksr (uint16_t value);

/* Write the carrier key-scale rate bit. */
void register_write_car_ksr (uint16_t value);

/* Write the modulator multiplication-factor register. */
void register_write_mod_multi (uint16_t value);

/* Write the carrier multiplication-factor register. */
void register_write_car_multi (uint16_t value);

/* Write the modulator key-scale level register. */
void register_write_mod_ksl (uint16_t value);

/* Write the carrier key-scale level register. */
void register_write_car_ksl (uint16_t value);

/* Write the total-level register. */
void register_write_total_level (uint16_t value);

/* Write the modulator waveform bit. */
void register_write_mod_waveform (uint16_t value);

/* Write the carrier waveform bit. */
void register_write_car_waveform (uint16_t value);

/* Write the feedback register. */
void register_write_feedback (uint16_t value);

/* Write the modulator attack-rate register. */
void register_write_mod_attack_rate (uint16_t value);

/* Write the carrier attack-rate register. */
void register_write_car_attack_rate (uint16_t value);

/* Write the modulator decay-rate register. */
void register_write_mod_decay_rate (uint16_t value);

/* Write the carrier decay-rate register. */
void register_write_car_decay_rate (uint16_t value);

/* Write the modulator sustain-level register. */
void register_write_mod_sustain_level (uint16_t value);

/* Write the carrier sustain-level register. */
void register_write_car_sustain_level (uint16_t value);

/* Write the modulator release-rate register. */
void register_write_mod_release_rate (uint16_t value);

/* Write the carrier release-rate register. */
void register_write_car_release_rate (uint16_t value);

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

