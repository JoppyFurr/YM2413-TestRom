
# YM2413 TestRom

This is a tool to configure and test the YM2413 sound chip used in the Japanese version of the Sega Master System.

This ROM is intended to help with emulator development. To allow the user to compare the behaviour of the real YM2413 sound chip against the emulator being tested.  The idea being, you can run this side-by-side on an emulator and also on a system with a real YM2413.

For melody sounds, all custom-instrument parameters can be adjusted, as well as volume, sustain, and instrument-select. Notes are played using an on-screen piano keyboard. When notes are pressed, the block and f-num being used are shown.

For rhythm sounds, the sustain, block, and fnum are configurable for the three underlying channels used by the rhythm implementation, as well as individual volumes and activation-buttons for the five rhythm sounds.

The interface is inspired by the look of '80s Yamaha synths, with 7-seg style displays. 

## Dependencies
 * zlib
