/*
 * YM2413 Test ROM
 * Joppy Furr 2023
 */

typedef enum element_type_e {
    TYPE_TAB = 0,
    TYPE_VALUE,
    TYPE_VALUE_WIDE,
    TYPE_LED,
    TYPE_BUTTON,
    TYPE_KEYBOARD
} element_type_t;

typedef enum element_id_e {

    /* Tabs */
    ELEMENT_MELODY_TAB = 0,
    ELEMENT_RHYTHM_TAB,

    /* Melody Mode elements */
    ELEMENT_INSTRUMENT,
    ELEMENT_VOLUME,
    ELEMENT_SUSTAIN,
    ELEMENT_FEEDBACK,
    ELEMENT_TOTAL_LEVEL,
    ELEMENT_MOD_MULTI,
    ELEMENT_MOD_KSR,
    ELEMENT_MOD_VIBRATO,
    ELEMENT_MOD_WAVEFORM,
    ELEMENT_MOD_ENVELOPE_TYPE,
    ELEMENT_MOD_AM,
    ELEMENT_MOD_KSL,
    ELEMENT_MOD_ATTACK_RATE,
    ELEMENT_MOD_DECAY_RATE,
    ELEMENT_MOD_SUSTAIN_LEVEL,
    ELEMENT_MOD_RELEASE_RATE,
    ELEMENT_CAR_MULTI,
    ELEMENT_CAR_KSR,
    ELEMENT_CAR_VIBRATO,
    ELEMENT_CAR_WAVEFORM,
    ELEMENT_CAR_ENVELOPE_TYPE,
    ELEMENT_CAR_AM,
    ELEMENT_CAR_KSL,
    ELEMENT_CAR_ATTACK_RATE,
    ELEMENT_CAR_DECAY_RATE,
    ELEMENT_CAR_SUSTAIN_LEVEL,
    ELEMENT_CAR_RELEASE_RATE,
    ELEMENT_KEYBOARD,

    /* Rhythm Mode elements */
    ELEMENT_CH6_BLOCK,
    ELEMENT_CH6_FNUM,
    ELEMENT_CH7_BLOCK,
    ELEMENT_CH7_FNUM,
    ELEMENT_CH8_BLOCK,
    ELEMENT_CH8_FNUM,
    ELEMENT_BD_VOLUME,
    ELEMENT_BD_BUTTON,
    ELEMENT_HH_VOLUME,
    ELEMENT_HH_BUTTON,
    ELEMENT_SD_VOLUME,
    ELEMENT_SD_BUTTON,
    ELEMENT_TT_VOLUME,
    ELEMENT_TT_BUTTON,
    ELEMENT_TC_VOLUME,
    ELEMENT_TC_BUTTON,

    ELEMENT_COUNT
} element_id_t;


typedef struct gui_element_s {

    element_type_t type;
    uint16_t max;

    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    element_id_t up;
    element_id_t down;
    element_id_t left;
    element_id_t right;

    void (*callback) (uint16_t value);

} gui_element_t;


static const gui_element_t melody_gui [ELEMENT_COUNT] = {
    [ELEMENT_MELODY_TAB] = {
        .type = TYPE_TAB, .max = 1,
        .x = 23, .y = 0, .width = 4, .height = 2,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_TOTAL_LEVEL,
        .left = ELEMENT_FEEDBACK,           .right = ELEMENT_RHYTHM_TAB
    },
    [ELEMENT_RHYTHM_TAB] = {
        .type = TYPE_TAB, .max = 1,
        .x = 27, .y = 0, .width = 5, .height = 2,
        .up   = ELEMENT_RHYTHM_TAB,         .down  = ELEMENT_TOTAL_LEVEL,
        .left = ELEMENT_MELODY_TAB,         .right = ELEMENT_RHYTHM_TAB
    },
    [ELEMENT_INSTRUMENT] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 4, .y = 2, .width = 2, .height = 3,
        .callback = register_write_instrument,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_MOD_MULTI,
        .left = ELEMENT_INSTRUMENT,         .right = ELEMENT_VOLUME
    },
    [ELEMENT_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 7, .y = 2, .width = 2, .height = 3,
        .callback = register_write_volume,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_MOD_KSR,
        .left = ELEMENT_INSTRUMENT,         .right = ELEMENT_SUSTAIN
    },
    [ELEMENT_SUSTAIN] = {
        .type = TYPE_LED, .max = 1,
        .x = 10, .y = 3, .width = 2, .height = 2,
        .callback = register_write_sustain,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_MOD_VIBRATO,
        .left = ELEMENT_VOLUME,             .right = ELEMENT_FEEDBACK
    },
    [ELEMENT_FEEDBACK] = {
        .type = TYPE_VALUE, .max = 7,
        .x = 19, .y = 3, .width = 4, .height = 2,
        .callback = register_write_feedback,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_MOD_ATTACK_RATE,
        .left = ELEMENT_SUSTAIN,            .right = ELEMENT_TOTAL_LEVEL
    },
    [ELEMENT_TOTAL_LEVEL] = {
        .type = TYPE_VALUE, .max = 63,
        .x = 25, .y = 3, .width = 5, .height = 2,
        .callback = register_write_total_level,
        .up   = ELEMENT_RHYTHM_TAB,         .down  = ELEMENT_MOD_SUSTAIN_LEVEL,
        .left = ELEMENT_FEEDBACK,           .right = ELEMENT_TOTAL_LEVEL
    },
    [ELEMENT_MOD_MULTI] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 4, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_multi,
        .up   = ELEMENT_INSTRUMENT,         .down  = ELEMENT_CAR_MULTI,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_KSR
    },
    [ELEMENT_MOD_KSR] = {
        .type = TYPE_LED, .max = 1,
        .x = 7, .y = 6, .width = 2, .height = 2,
        .callback = register_write_mod_ksr,
        .up   = ELEMENT_VOLUME,             .down  = ELEMENT_MOD_ENVELOPE_TYPE,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_VIBRATO
    },
    [ELEMENT_MOD_VIBRATO] = {
        .type  = TYPE_LED, .max = 1,
        .x = 10, .y = 6, .width = 2, .height = 2,
        .callback = register_write_mod_vibrato,
        .up    = ELEMENT_SUSTAIN,           .down  = ELEMENT_MOD_AM,
        .left  = ELEMENT_MOD_KSR,           .right = ELEMENT_MOD_WAVEFORM
    },
    [ELEMENT_MOD_WAVEFORM] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_waveform,
        .up   = ELEMENT_SUSTAIN,            .down  = ELEMENT_CAR_WAVEFORM,
        .left = ELEMENT_MOD_VIBRATO,        .right = ELEMENT_MOD_KSL
    },
    [ELEMENT_MOD_ENVELOPE_TYPE] = {
        .type = TYPE_LED, .max = 1,
        .x = 7, .y = 8, .width = 2, .height = 2,
        .callback = register_write_mod_eg_type,
        .up   = ELEMENT_MOD_KSR,            .down  = ELEMENT_CAR_KSR,
        .left = ELEMENT_MOD_MULTI,          .right = ELEMENT_MOD_AM
    },
    [ELEMENT_MOD_AM] = {
        .type = TYPE_LED, .max = 1,
        .x = 10, .y = 8, .width = 2, .height = 2,
        .callback = register_write_mod_am,
        .up   = ELEMENT_MOD_VIBRATO,        .down  = ELEMENT_CAR_VIBRATO,
        .left = ELEMENT_MOD_ENVELOPE_TYPE,  .right = ELEMENT_MOD_WAVEFORM
    },
    [ELEMENT_MOD_KSL] = {
        .type = TYPE_VALUE, .max = 3,
        .x = 16, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_ksl,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_KSL,
        .left = ELEMENT_MOD_WAVEFORM,       .right = ELEMENT_MOD_ATTACK_RATE
    },
    [ELEMENT_MOD_ATTACK_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 19, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_attack_rate,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_ATTACK_RATE,
        .left = ELEMENT_MOD_KSL,            .right = ELEMENT_MOD_DECAY_RATE
    },
    [ELEMENT_MOD_DECAY_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 22, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_decay_rate,
        .up   = ELEMENT_FEEDBACK,           .down  = ELEMENT_CAR_DECAY_RATE,
        .left = ELEMENT_MOD_ATTACK_RATE,    .right = ELEMENT_MOD_SUSTAIN_LEVEL
    },
    [ELEMENT_MOD_SUSTAIN_LEVEL] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 25, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_sustain_level,
        .up   = ELEMENT_TOTAL_LEVEL,        .down  = ELEMENT_CAR_SUSTAIN_LEVEL,
        .left = ELEMENT_MOD_DECAY_RATE,     .right = ELEMENT_MOD_RELEASE_RATE
    },
    [ELEMENT_MOD_RELEASE_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 28, .y = 6, .width = 2, .height = 3,
        .callback = register_write_mod_release_rate,
        .up   = ELEMENT_TOTAL_LEVEL,        .down  = ELEMENT_CAR_RELEASE_RATE,
        .left = ELEMENT_MOD_SUSTAIN_LEVEL,  .right = ELEMENT_MOD_RELEASE_RATE
    },
    [ELEMENT_CAR_MULTI] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 4, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_multi,
        .up   = ELEMENT_MOD_MULTI,          .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_KSR
    },
    [ELEMENT_CAR_KSR] = {
        .type = TYPE_LED, .max = 1,
        .x = 7, .y = 11, .width = 2, .height = 2,
        .callback = register_write_car_ksr,
        .up   = ELEMENT_MOD_ENVELOPE_TYPE,  .down  = ELEMENT_CAR_ENVELOPE_TYPE,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_VIBRATO
    },
    [ELEMENT_CAR_VIBRATO] = {
        .type = TYPE_LED, .max = 1,
        .x = 10, .y = 11, .width = 2, .height = 2,
        .callback = register_write_car_vibrato,
        .up   = ELEMENT_MOD_AM,             .down  = ELEMENT_CAR_AM,
        .left = ELEMENT_CAR_KSR,            .right = ELEMENT_CAR_WAVEFORM
    },
    [ELEMENT_CAR_WAVEFORM] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_waveform,
        .up   = ELEMENT_MOD_WAVEFORM,       .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_VIBRATO,        .right = ELEMENT_CAR_KSL
    },
    [ELEMENT_CAR_ENVELOPE_TYPE] = {
        .type = TYPE_LED, .max = 1,
        .x = 7, .y = 13, .width = 2, .height = 2,
        .callback = register_write_car_eg_type,
        .up   = ELEMENT_CAR_KSR,            .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_MULTI,          .right = ELEMENT_CAR_AM
    },
    [ELEMENT_CAR_AM] = {
        .type = TYPE_LED, .max = 1,
        .x = 10, .y = 13, .width = 2, .height = 2,
        .callback = register_write_car_am,
        .up   = ELEMENT_CAR_VIBRATO,        .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_ENVELOPE_TYPE, .right = ELEMENT_CAR_WAVEFORM
    },
    [ELEMENT_CAR_KSL] = {
        .type = TYPE_VALUE, .max = 3,
        .x = 16, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_ksl,
        .up   = ELEMENT_MOD_KSL,            .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_WAVEFORM,       .right = ELEMENT_CAR_ATTACK_RATE
    },
    [ELEMENT_CAR_ATTACK_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 19, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_attack_rate,
        .up   = ELEMENT_MOD_ATTACK_RATE,    .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_KSL,            .right = ELEMENT_CAR_DECAY_RATE
    },
    [ELEMENT_CAR_DECAY_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 22, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_decay_rate,
        .up   = ELEMENT_MOD_DECAY_RATE,     .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_ATTACK_RATE,    .right = ELEMENT_CAR_SUSTAIN_LEVEL
    },
    [ELEMENT_CAR_SUSTAIN_LEVEL] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 25, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_sustain_level,
        .up   = ELEMENT_MOD_SUSTAIN_LEVEL, .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_DECAY_RATE,     .right = ELEMENT_CAR_RELEASE_RATE
    },
    [ELEMENT_CAR_RELEASE_RATE] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 28, .y = 11, .width = 2, .height = 3,
        .callback = register_write_car_release_rate,
        .up   = ELEMENT_MOD_RELEASE_RATE,   .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_CAR_SUSTAIN_LEVEL,  .right = ELEMENT_CAR_RELEASE_RATE
    },
    [ELEMENT_KEYBOARD] = {
        .type = TYPE_KEYBOARD, .height = 0,
        .down = ELEMENT_KEYBOARD,
        .left = ELEMENT_KEYBOARD,           .right = ELEMENT_KEYBOARD
    }
};

static const gui_element_t rhythm_gui [ELEMENT_COUNT] = {
    [ELEMENT_MELODY_TAB] = {
        .type = TYPE_TAB, .max = 1,
        .x = 23, .y = 0, .width = 4, .height = 2,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_BD_BUTTON,
        .left = ELEMENT_BD_BUTTON,          .right = ELEMENT_RHYTHM_TAB
    },
    [ELEMENT_RHYTHM_TAB] = {
        .type = TYPE_TAB, .max = 1,
        .x = 27, .y = 0, .width = 5, .height = 2,
        .up   = ELEMENT_RHYTHM_TAB,         .down  = ELEMENT_BD_BUTTON,
        .left = ELEMENT_MELODY_TAB,         .right = ELEMENT_RHYTHM_TAB
    },
    [ELEMENT_CH6_BLOCK] = {
        .type = TYPE_VALUE, .max = 7,
        .x = 6, .y = 4, .width = 2, .height = 3,
        .callback = register_write_ch6_block,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_CH7_BLOCK,
        .left = ELEMENT_CH6_BLOCK,          .right = ELEMENT_CH6_FNUM
    },
    [ELEMENT_CH6_FNUM] = {
        .type = TYPE_VALUE_WIDE, .max = 511,
        .x = 9, .y = 4, .width = 3, .height = 3,
        .callback = register_write_ch6_fnum,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_CH7_FNUM,
        .left = ELEMENT_CH6_BLOCK,          .right = ELEMENT_BD_VOLUME
    },
    [ELEMENT_CH7_BLOCK] = {
        .x = 6, .y = 10, .width = 2, .height = 3,
        .type = TYPE_VALUE, .max = 7,
        .callback = register_write_ch7_block,
        .up   = ELEMENT_CH6_BLOCK,          .down  = ELEMENT_CH8_BLOCK,
        .left = ELEMENT_CH7_BLOCK,          .right = ELEMENT_CH7_FNUM
    },
    [ELEMENT_CH7_FNUM] = {
        .type = TYPE_VALUE_WIDE, .max = 511,
        .x = 9, .y = 10, .width = 3, .height = 3,
        .callback = register_write_ch7_fnum,
        .up   = ELEMENT_CH6_FNUM,           .down  = ELEMENT_CH8_FNUM,
        .left = ELEMENT_CH7_BLOCK,          .right = ELEMENT_SD_VOLUME
    },
    [ELEMENT_CH8_BLOCK] = {
        .x = 6, .y = 18, .width = 2, .height = 3,
        .type = TYPE_VALUE, .max = 7,
        .callback = register_write_ch8_block,
        .up   = ELEMENT_CH7_BLOCK,          .down  = ELEMENT_CH8_BLOCK,
        .left = ELEMENT_CH8_BLOCK,          .right = ELEMENT_CH8_FNUM
    },
    [ELEMENT_CH8_FNUM] = {
        .type = TYPE_VALUE_WIDE, .max = 511,
        .x = 9, .y = 18, .width = 3, .height = 3,
        .callback = register_write_ch8_fnum,
        .up   = ELEMENT_CH7_FNUM,           .down  = ELEMENT_CH8_FNUM,
        .left = ELEMENT_CH8_BLOCK,          .right = ELEMENT_TC_VOLUME
    },
    [ELEMENT_BD_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 15, .y = 4, .width = 2, .height = 3,
        .callback = register_write_bd_volume,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_HH_VOLUME,
        .left = ELEMENT_CH6_FNUM,           .right = ELEMENT_BD_BUTTON
    },
    [ELEMENT_BD_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 19, .y = 4, .width = 2, .height = 2,
        .callback = register_write_bd_key,
        .up   = ELEMENT_MELODY_TAB,         .down  = ELEMENT_HH_BUTTON,
        .left = ELEMENT_BD_VOLUME,          .right = ELEMENT_BD_BUTTON
    },
    [ELEMENT_HH_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 15, .y = 8, .width = 2, .height = 3,
        .callback = register_write_hh_volume,
        .up   = ELEMENT_BD_VOLUME,          .down  = ELEMENT_SD_VOLUME,
        .left = ELEMENT_CH7_FNUM,           .right = ELEMENT_HH_BUTTON
    },
    [ELEMENT_HH_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 19, .y = 8, .width = 2, .height = 2,
        .callback = register_write_hh_key,
        .up   = ELEMENT_BD_BUTTON,          .down  = ELEMENT_SD_BUTTON,
        .left = ELEMENT_HH_VOLUME,          .right = ELEMENT_HH_BUTTON
    },
    [ELEMENT_SD_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 15, .y = 12, .width = 2, .height = 3,
        .callback = register_write_sd_volume,
        .up   = ELEMENT_HH_VOLUME,          .down  = ELEMENT_TT_VOLUME,
        .left = ELEMENT_CH7_FNUM,           .right = ELEMENT_SD_BUTTON
    },
    [ELEMENT_SD_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 19, .y = 12, .width = 2, .height = 2,
        .callback = register_write_sd_key,
        .up   = ELEMENT_HH_BUTTON,          .down  = ELEMENT_TT_BUTTON,
        .left = ELEMENT_SD_VOLUME,          .right = ELEMENT_SD_BUTTON
    },
    [ELEMENT_TT_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 15, .y = 16, .width = 2, .height = 3,
        .callback = register_write_tt_volume,
        .up   = ELEMENT_SD_VOLUME,          .down  = ELEMENT_TC_VOLUME,
        .left = ELEMENT_CH8_FNUM,           .right = ELEMENT_TT_BUTTON
    },
    [ELEMENT_TT_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 19, .y = 16, .width = 2, .height = 2,
        .callback = register_write_tt_key,
        .up   = ELEMENT_SD_BUTTON,          .down  = ELEMENT_TC_BUTTON,
        .left = ELEMENT_TT_VOLUME,          .right = ELEMENT_TT_BUTTON
    },
    [ELEMENT_TC_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 15, .y = 20, .width = 2, .height = 3,
        .callback = register_write_tc_volume,
        .up   = ELEMENT_TT_VOLUME,          .down  = ELEMENT_TC_VOLUME,
        .left = ELEMENT_CH8_FNUM,           .right = ELEMENT_TC_BUTTON
    },
    [ELEMENT_TC_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 19, .y = 20, .width = 2, .height = 2,
        .callback = register_write_tc_key,
        .up   = ELEMENT_TT_BUTTON,          .down  = ELEMENT_TC_BUTTON,
        .left = ELEMENT_TC_VOLUME,          .right = ELEMENT_TC_BUTTON
    },
};
