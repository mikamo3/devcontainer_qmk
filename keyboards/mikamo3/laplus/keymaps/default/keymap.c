// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,_LOWER,_RAISE
};

enum custom_keycodes {
    BASE = SAFE_RANGE,
    KC_LOWER,
    KC_RAISE,
};

#define SPC_LWR LT(_LOWER,KC_SPC)
#define ENT_RSE LT(_RAISE,KC_ENT)
#define ESC_CTL CTL_T(KC_ESC)
#define TAB_SFT SFT_T(KC_TAB)
#define BSP_SFT RSFT_T(KC_BSPC)
#define BSP_CTL RCTL_T(KC_BSPC)
#define BSH_SFT RSFT_T(KC_BSLS)
#define KANA LCTL(KC_SPACE)
#define GUI_H LGUI(KC_H)
#define GUI_L LGUI(KC_L)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split(
        ESC_CTL, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_MINS,       KC_EQL,  KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,    BSP_CTL,
        TAB_SFT, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_LBRC,       KC_RBRC, KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, BSH_SFT,
                 KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   GUI_H,         GUI_L,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,
                                 KC_LALT,KC_LGUI,KC_LOWER,ESC_CTL,      BSP_SFT, KC_RAISE,KC_RGUI,KC_RALT
    ),
    [_LOWER] = LAYOUT_split(
        KANA, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_DQUO,  KC_QUOT, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
        KANA,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LCBR,  KC_RCBR, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
                 _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______,  KC_DEL,  _______, _______, _______

    ),
    [_RAISE] = LAYOUT_split(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
        _______,    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,  _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,_______, _______,
                 KC_PSCR, KC_INS,  KC_SCRL, KC_PAUS, _______, _______,  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______,  _______, _______, _______, _______

    )
};
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            register_code(KC_LGUI);
            tap_code(KC_LEFT);
            unregister_code(KC_LGUI);
        } else {
            register_code(KC_LGUI);
            tap_code(KC_RIGHT);
            unregister_code(KC_LGUI);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            register_code(KC_LGUI);
            tap_code(KC_DOWN);
            unregister_code(KC_LGUI);
        } else {
            register_code(KC_LGUI);
            tap_code(KC_UP);
            unregister_code(KC_LGUI);
        }
    }
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;
    static bool     lower_pressed    = false;
    static bool     raise_pressed    = false;

    switch (keycode) {
        case KC_LOWER:
            if (record->event.pressed) {
                key_timer = timer_read32();
                layer_on(_LOWER);
                lower_pressed = true;
            } else {
                if (lower_pressed && timer_elapsed32(key_timer) <= TAPPING_TERM) {
                    register_code(KC_SPC);
                    unregister_code(KC_SPC);
                }
                layer_off(_LOWER);
                lower_pressed = false;
            }
            return false;
            break;
        case KC_RAISE:
            if (record->event.pressed) {
                key_timer = timer_read32();
                layer_on(_RAISE);
                raise_pressed = true;
            } else {
                if (raise_pressed && timer_elapsed32(key_timer) <= TAPPING_TERM) {
                    register_code(KC_ENT);
                    unregister_code(KC_ENT);
                }
                layer_off(_RAISE);
                raise_pressed = false;
            }
            return false;
            break;
        default:
            if (record->event.pressed) {
                lower_pressed    = false;
                raise_pressed    = false;
            }
            break;
    }
    return true;
}
