/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

#include <stdint.h>
__sfr __at 0xf0 ym2413_addr;
__sfr __at 0xf1 ym2413_data;

typedef struct channel_regs_s {
    uint8_t r10;
    uint8_t r20;
    uint8_t r30;
} channel_regs_t;

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
 * Write the sustain register.
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
 * Write the key register.
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

