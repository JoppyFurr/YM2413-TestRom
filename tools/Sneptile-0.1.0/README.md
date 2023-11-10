
# Sneptile
Sneptile is a tool for converting images into tile data for the Sega Master System.

Input images should have a width and height that are multiples of 8px.
Tiles are generated left-to-right, top-to-bottom, first file to last file.

Usage: `./Sneptile --output tile_data --palette 0x04 0x19 empty.png cursor.png`

 * `--output <dir>`: specifies the directory for the generated files
 * `--palette <0x...>`: specifies the first n entries of the palette
 * `... <.png>`: the remaining parameters are `.png` images to generate tiles from

The following three files are generated in the specified output directory:

pattern.h contains the pattern data to load into the VDP:
```
const uint32_t patterns [] = {

    /* empty.png */
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,

    /* cursor.png */
    0x0000c000, 0x0000e040, 0x0000f060, 0x0000f870, 0x0000fc78, 0x0000fe7c, 0x0000ff7e, 0x0000ff7f,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00008000,
    0x0000ff7f, 0x0000ff7c, 0x0000fe6c, 0x0000ef46, 0x0000cf06, 0x00008703, 0x00000703, 0x00000300,
    0x0000c080, 0x0000e000, 0x00000000, 0x00000000, 0x00000000, 0x00008000, 0x00008000, 0x00008000,
    0x00008080, 0x00804040, 0x00c02060, 0x00e01070, 0x00b04838, 0x00b8443c, 0x009c621e, 0x009e611f,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x008f700f, 0x00906f1f, 0x00a45a36, 0x00ca2563, 0x008a4543, 0x00058281, 0x00070003, 0x00000303,
    0x00008080, 0x0000c0c0, 0x00000000, 0x00000000, 0x00000000, 0x00008080, 0x00008080, 0x00008080,
    0x0000c0c0, 0x0000e0a0, 0x0000f090, 0x0000f888, 0x0000fc84, 0x0000fe82, 0x0000ff81, 0x0000ff80,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00008080,
    0x0000ff80, 0x0000ff83, 0x0000fe92, 0x0000efa9, 0x0000cfc9, 0x00008784, 0x00000704, 0x00000303,
    0x0000c040, 0x0000e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00008080, 0x00008080, 0x00008080,
};
```

pattern_index.h contains the index of the first tile from each image file:
```
#define PATTERN_EMPTY 0
#define PATTERN_CURSOR 1

```

palette.h contains the palette:
```
const uint8_t palette [16] = { 0x04, 0x19, 0x3f, 0x00, 0x15, 0x2a };

```

## Dependencies
 * zlib
