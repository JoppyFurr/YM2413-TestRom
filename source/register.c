/*
 * YM2413 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>
__sfr __at 0xf0 ym2413_addr;
__sfr __at 0xf1 ym2413_data;
__sfr __at 0xf2 audio_control;

typedef struct instrument_regs_s {
    uint8_t r00;
    uint8_t r01;
    uint8_t r02;
    uint8_t r03;
    uint8_t r04;
    uint8_t r05;
    uint8_t r06;
    uint8_t r07;
} instrument_regs_t;

typedef struct channel_regs_s {
    uint8_t r0e;
    uint8_t r10;
    uint8_t r16;
    uint8_t r17;
    uint8_t r18;
    uint8_t r20;
    uint8_t r26;
    uint8_t r27;
    uint8_t r28;
    uint8_t r30;
    uint8_t r36;
    uint8_t r37;
    uint8_t r38;
} channel_regs_t;

static instrument_regs_t instrument_regs = { 0 };
static channel_regs_t channel_regs = { 0 };


/*
 * Use the audio control register to take the YM2413 out of mute.
 */
void enable_fm (void)
{
    audio_control = 0x01;
}


/*
 * Write to a register on the YM2413.
 */
static void register_write (uint8_t addr, uint8_t value)
{
    ym2413_addr = addr;
    __asm
        /* 12 cycle delay after setting address. */
        nop
        nop
        nop
    __endasm;
    ym2413_data = value;
    __asm
        /* 84 cycle delay after writing register. */
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    __endasm;
}


/*
 * Write the modulator AM bit.
 */
void register_write_mod_am (uint16_t value)
{
    instrument_regs.r00 &= 0x7f;
    instrument_regs.r00 |= (value << 7) & 0x80;
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier AM bit.
 */
void register_write_car_am (uint16_t value)
{
    instrument_regs.r01 &= 0x7f;
    instrument_regs.r01 |= (value << 7) & 0x80;
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator vibrato bit.
 */
void register_write_mod_vibrato (uint16_t value)
{
    instrument_regs.r00 &= 0xbf;
    instrument_regs.r00 |= (value << 6) & 0x40;
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier vibrato bit.
 */
void register_write_car_vibrato (uint16_t value)
{
    instrument_regs.r01 &= 0xbf;
    instrument_regs.r01 |= (value << 6) & 0x40;
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator envelope-type bit.
 */
void register_write_mod_eg_type (uint16_t value)
{
    instrument_regs.r00 &= 0xdf;
    instrument_regs.r00 |= (value << 5) & 0x20;
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier envelope-type bit.
 */
void register_write_car_eg_type (uint16_t value)
{
    instrument_regs.r01 &= 0xdf;
    instrument_regs.r01 |= (value << 5) & 0x20;
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator key-scale rate bit.
 */
void register_write_mod_ksr (uint16_t value)
{
    instrument_regs.r00 &= 0xef;
    instrument_regs.r00 |= (value << 4) & 0x10;
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier key-scale rate bit.
 */
void register_write_car_ksr (uint16_t value)
{
    instrument_regs.r01 &= 0xef;
    instrument_regs.r01 |= (value << 4) & 0x10;
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator multiplication-factor register.
 */
void register_write_mod_multi (uint16_t value)
{
    instrument_regs.r00 &= 0xf0;
    instrument_regs.r00 |= (value & 0x0f);
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier multiplication-factor register.
 */
void register_write_car_multi (uint16_t value)
{
    instrument_regs.r01 &= 0xf0;
    instrument_regs.r01 |= (value & 0x0f);
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator key-scale level register.
 */
void register_write_mod_ksl (uint16_t value)
{
    instrument_regs.r02 &= 0x3f;
    instrument_regs.r02 |= (value << 6) & 0xc0;
    register_write (0x02, instrument_regs.r02);
}


/*
 * Write the carrier key-scale level register.
 */
void register_write_car_ksl (uint16_t value)
{
    instrument_regs.r03 &= 0x3f;
    instrument_regs.r03 |= (value << 6) & 0xc0;
    register_write (0x03, instrument_regs.r03);
}


/*
 * Write the total-level register.
 */
void register_write_total_level (uint16_t value)
{
    instrument_regs.r02 &= 0xc0;
    instrument_regs.r02 |= value & 0x3f;
    register_write (0x02, instrument_regs.r02);
}


/*
 * Write the modulator waveform bit.
 */
void register_write_mod_waveform (uint16_t value)
{
    instrument_regs.r03 &= 0xf7;
    instrument_regs.r03 |= (value << 3) & 0x08;
    register_write (0x03, instrument_regs.r03);
}


/*
 * Write the carrier waveform bit.
 */
void register_write_car_waveform (uint16_t value)
{
    instrument_regs.r03 &= 0xef;
    instrument_regs.r03 |= (value << 4) & 0x10;
    register_write (0x03, instrument_regs.r03);
}


/*
 * Write the feedback register.
 */
void register_write_feedback (uint16_t value)
{
    instrument_regs.r03 &= 0xf8;
    instrument_regs.r03 |= value & 0x07;
    register_write (0x03, instrument_regs.r03);
}


/*
 * Write the modulator attack-rate register.
 */
void register_write_mod_attack_rate (uint16_t value)
{
    instrument_regs.r04 &= 0x0f;
    instrument_regs.r04 |= (value << 4) & 0xf0;
    register_write (0x04, instrument_regs.r04);
}


/*
 * Write the carrier attack-rate register.
 */
void register_write_car_attack_rate (uint16_t value)
{
    instrument_regs.r05 &= 0x0f;
    instrument_regs.r05 |= (value << 4) & 0xf0;
    register_write (0x05, instrument_regs.r05);
}


/*
 * Write the modulator decay-rate register.
 */
void register_write_mod_decay_rate (uint16_t value)
{
    instrument_regs.r04 &= 0xf0;
    instrument_regs.r04 |= value & 0x0f;
    register_write (0x04, instrument_regs.r04);
}


/*
 * Write the carrier decay-rate register.
 */
void register_write_car_decay_rate (uint16_t value)
{
    instrument_regs.r05 &= 0xf0;
    instrument_regs.r05 |= value & 0x0f;
    register_write (0x05, instrument_regs.r05);
}


/*
 * Write the modulator sustain-level register.
 */
void register_write_mod_sustain_level (uint16_t value)
{
    instrument_regs.r06 &= 0x0f;
    instrument_regs.r06 |= (value << 4) & 0xf0;
    register_write (0x06, instrument_regs.r06);
}


/*
 * Write the carrier sustain-level register.
 */
void register_write_car_sustain_level (uint16_t value)
{
    instrument_regs.r07 &= 0x0f;
    instrument_regs.r07 |= (value << 4) & 0xf0;
    register_write (0x07, instrument_regs.r07);
}


/*
 * Write the modulator release-rate register.
 */
void register_write_mod_release_rate (uint16_t value)
{
    instrument_regs.r06 &= 0xf0;
    instrument_regs.r06 |= value & 0x0f;
    register_write (0x06, instrument_regs.r06);
}


/*
 * Write the carrier release-rate register.
 */
void register_write_car_release_rate (uint16_t value)
{
    instrument_regs.r07 &= 0xf0;
    instrument_regs.r07 |= value & 0x0f;
    register_write (0x07, instrument_regs.r07);
}


/*
 * Update the melody f-num and block registers.
 */
void register_write_fnum_block (uint16_t fnum, uint8_t block)
{
    channel_regs.r10 = fnum;
    register_write (0x10, channel_regs.r10);

    channel_regs.r20 &= 0xf0;
    channel_regs.r20 |= (fnum  >> 8) & 0x01;
    channel_regs.r20 |= (block << 1) & 0x0e;
    register_write (0x20, channel_regs.r20);
}


/*
 * Write the bass-drum sustain register.
 */
void register_write_ch6_sustain (uint16_t value)
{
    channel_regs.r26 &= 0xdf;
    channel_regs.r26 |= (value << 5) & 0x20;
    register_write (0x26, channel_regs.r26);
}


/*
 * Write the bass-drum block register.
 */
void register_write_ch6_block (uint16_t value)
{
    channel_regs.r26 &= 0xf1;
    channel_regs.r26 |= (value << 1) & 0x0e;
    register_write (0x26, channel_regs.r26);
}


/*
 * Write the bass-drum fnum register.
 */
void register_write_ch6_fnum (uint16_t value)
{
    channel_regs.r16 = value;
    register_write (0x16, channel_regs.r16);

    channel_regs.r26 &= 0xfe;
    channel_regs.r26 |= (value >> 8) & 0x01;
    register_write (0x26, channel_regs.r26);
}


/*
 * Write the high-hat / snare-drum sustain register.
 */
void register_write_ch7_sustain (uint16_t value)
{
    channel_regs.r27 &= 0xdf;
    channel_regs.r27 |= (value << 5) & 0x20;
    register_write (0x27, channel_regs.r27);
}


/*
 * Write the high-hat / snare-drum block register.
 */
void register_write_ch7_block (uint16_t value)
{
    channel_regs.r27 &= 0xf1;
    channel_regs.r27 |= (value << 1) & 0x0e;
    register_write (0x27, channel_regs.r27);
}


/*
 * Write the high-hat / snare-drum fnum register.
 */
void register_write_ch7_fnum (uint16_t value)
{
    channel_regs.r17 = value;
    register_write (0x17, channel_regs.r17);

    channel_regs.r27 &= 0xfe;
    channel_regs.r27 |= (value >> 8) & 0x01;
    register_write (0x27, channel_regs.r27);
}


/*
 * Write the tom-tom / top-cymbal sustain register.
 */
void register_write_ch8_sustain (uint16_t value)
{
    channel_regs.r28 &= 0xdf;
    channel_regs.r28 |= (value << 5) & 0x20;
    register_write (0x28, channel_regs.r28);
}


/*
 * Write the tom-tom / top-cymbal block register.
 */
void register_write_ch8_block (uint16_t value)
{
    channel_regs.r28 &= 0xf1;
    channel_regs.r28 |= (value << 1) & 0x0e;
    register_write (0x28, channel_regs.r28);
}


/*
 * Write the tom-tom / top-cymbal fnum register.
 */
void register_write_ch8_fnum (uint16_t value)
{
    channel_regs.r18 = value;
    register_write (0x18, channel_regs.r18);

    channel_regs.r28 &= 0xfe;
    channel_regs.r28 |= (value >> 8) & 0x01;
    register_write (0x28, channel_regs.r28);
}


/*
 * Write the bass-drum volume.
 */
void register_write_bd_volume (uint16_t value)
{
    channel_regs.r36 &= 0xf0;
    channel_regs.r36 |= value & 0x0f;
    register_write (0x36, channel_regs.r36);
}


/*
 * Write the high-hat volume.
 */
void register_write_hh_volume (uint16_t value)
{
    channel_regs.r37 &= 0x0f;
    channel_regs.r37 |= (value << 4) & 0xf0;
    register_write (0x37, channel_regs.r37);
}


/*
 * Write the snare-drum volume.
 */
void register_write_sd_volume (uint16_t value)
{
    channel_regs.r37 &= 0xf0;
    channel_regs.r37 |= value & 0x0f;
    register_write (0x37, channel_regs.r37);
}


/*
 * Write the tom-tom volume.
 */
void register_write_tt_volume (uint16_t value)
{
    channel_regs.r38 &= 0x0f;
    channel_regs.r38 |= (value << 4) & 0xf0;
    register_write (0x38, channel_regs.r38);
}


/*
 * Write the top-cymbal volume.
 */
void register_write_tc_volume (uint16_t value)
{
    channel_regs.r38 &= 0xf0;
    channel_regs.r38 |= value & 0x0f;
    register_write (0x38, channel_regs.r38);
}


/*
 * Write the rhythm register to key the bass-drum.
 */
void register_write_rhythm_mode (uint16_t value)
{
    channel_regs.r0e &= 0xdf;
    channel_regs.r0e |= (value << 5) & 0x20;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the rhythm register to key the bass-drum.
 */
void register_write_bd_key (uint16_t value)
{
    channel_regs.r0e &= 0xef;
    channel_regs.r0e |= (value << 4) & 0x10;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the rhythm register to key the high-hat.
 */
void register_write_hh_key (uint16_t value)
{
    channel_regs.r0e &= 0xfe;
    channel_regs.r0e |= value & 0x01;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the rhythm register to key the snare-drum.
 */
void register_write_sd_key (uint16_t value)
{
    channel_regs.r0e &= 0xf7;
    channel_regs.r0e |= (value << 3) & 0x08;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the rhythm register to key the tom-tom.
 */
void register_write_tt_key (uint16_t value)
{
    channel_regs.r0e &= 0xfb;
    channel_regs.r0e |= (value << 2) & 0x04;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the rhythm register to key the top-cymbal.
 */
void register_write_tc_key (uint16_t value)
{
    channel_regs.r0e &= 0xfd;
    channel_regs.r0e |= (value << 1) & 0x02;
    register_write (0x0e, channel_regs.r0e);
}


/*
 * Write the channel sustain bit.
 */
void register_write_sustain (uint16_t value)
{
    channel_regs.r20 &= 0xdf;
    channel_regs.r20 |= (value << 5) & 0x20;
    register_write (0x20, channel_regs.r20);
}


/*
 * Read the stored copy of the key_on bit.
 */
bool register_stored_key_on (void)
{
    return (channel_regs.r20 >> 4) & 0x01;
}


/*
 * Write the channel key-on bit.
 */
void register_write_key_on (uint16_t value)
{
    channel_regs.r20 &= 0xef;
    channel_regs.r20 |= (value << 4) & 0x10;
    register_write (0x20, channel_regs.r20);
}


/*
 * Write the instrument register.
 */
void register_write_instrument (uint16_t value)
{
    channel_regs.r30 &= 0x0f;
    channel_regs.r30 |= (value << 4) & 0xf0;
    register_write (0x30, channel_regs.r30);
}


/*
 * Write the volume register.
 */
void register_write_volume (uint16_t value)
{
    channel_regs.r30 &= 0xf0;
    channel_regs.r30 |= value & 0x0f;
    register_write (0x30, channel_regs.r30);
}

