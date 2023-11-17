/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdint.h>
__sfr __at 0xf0 ym2413_addr;
__sfr __at 0xf1 ym2413_data;

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
    uint8_t r10;
    uint8_t r20;
    uint8_t r30;
} channel_regs_t;

static instrument_regs_t instrument_regs = { 0 };
static channel_regs_t channel_regs = { 0 };


/*
 * Write to a register on the YM2413.
 */
static void register_write (uint8_t addr, uint8_t value)
{
    /* TODO: Do we need to enforce a delay between writes? */
    ym2413_addr = addr;
    ym2413_data = value;
}


/*
 * Write the modulator AM bit.
 */
#define AM_BIT 0x80
void register_write_mod_am (uint16_t value)
{
    if (value)
    {
        instrument_regs.r00 |= AM_BIT;
    }
    else
    {
        instrument_regs.r00 &= ~AM_BIT;
    }
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier AM bit.
 */
void register_write_car_am (uint16_t value)
{
    if (value)
    {
        instrument_regs.r01 |= AM_BIT;
    }
    else
    {
        instrument_regs.r01 &= ~AM_BIT;
    }
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator vibrato bit.
 */
#define VIBRATO_BIT 0x40
void register_write_mod_vibrato (uint16_t value)
{
    if (value)
    {
        instrument_regs.r00 |= VIBRATO_BIT;
    }
    else
    {
        instrument_regs.r00 &= ~VIBRATO_BIT;
    }
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier vibrato bit.
 */
void register_write_car_vibrato (uint16_t value)
{
    if (value)
    {
        instrument_regs.r01 |= VIBRATO_BIT;
    }
    else
    {
        instrument_regs.r01 &= ~VIBRATO_BIT;
    }
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator envelope-type bit.
 */
#define EGT_BIT 0x20
void register_write_mod_eg_type (uint16_t value)
{
    if (value)
    {
        instrument_regs.r00 |= EGT_BIT;
    }
    else
    {
        instrument_regs.r00 &= ~EGT_BIT;
    }
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier envelope-type bit.
 */
void register_write_car_eg_type (uint16_t value)
{
    if (value)
    {
        instrument_regs.r01 |= EGT_BIT;
    }
    else
    {
        instrument_regs.r01 &= ~EGT_BIT;
    }
    register_write (0x01, instrument_regs.r01);
}


/*
 * Write the modulator key-scale rate bit.
 */
#define KSR_BIT 0x10
void register_write_mod_ksr (uint16_t value)
{
    if (value)
    {
        instrument_regs.r00 |= KSR_BIT;
    }
    else
    {
        instrument_regs.r00 &= ~KSR_BIT;
    }
    register_write (0x00, instrument_regs.r00);
}


/*
 * Write the carrier key-scale rate bit.
 */
void register_write_car_ksr (uint16_t value)
{
    if (value)
    {
        instrument_regs.r01 |= KSR_BIT;
    }
    else
    {
        instrument_regs.r01 &= ~KSR_BIT;
    }
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
#define MOD_WF_BIT 0x08
void register_write_mod_waveform (uint16_t value)
{
    if (value)
    {
        instrument_regs.r03 |= MOD_WF_BIT;
    }
    else
    {
        instrument_regs.r03 &= ~MOD_WF_BIT;
    }
    register_write (0x03, instrument_regs.r03);
}


/*
 * Write the carrier waveform bit.
 */
#define CAR_WF_BIT 0x10
void register_write_car_waveform (uint16_t value)
{
    if (value)
    {
        instrument_regs.r03 |= CAR_WF_BIT;
    }
    else
    {
        instrument_regs.r03 &= ~CAR_WF_BIT;
    }
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
 * Update the f-num and block registers.
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
 * Write the channel sustain bit.
 */
#define SUSTAIN_BIT 0x20
void register_write_sustain (uint16_t value)
{
    if (value)
    {
        channel_regs.r20 |= SUSTAIN_BIT;
    }
    else
    {
        channel_regs.r20 &= ~SUSTAIN_BIT;
    }
    register_write (0x20, channel_regs.r20);
}


/*
 * Write the channel key-on bit.
 */
#define KEY_BIT 0x10
void register_write_key_on (uint16_t value)
{
    if (value)
    {
        channel_regs.r20 |= KEY_BIT;
    }
    else
    {
        channel_regs.r20 &= ~KEY_BIT;
    }
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

