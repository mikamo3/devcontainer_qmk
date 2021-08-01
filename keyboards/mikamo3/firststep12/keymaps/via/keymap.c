/* Copyright 2020 mikamo3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _EESCHEMA,
    _PCBNEW,
    _LIBRECAD,
    _NUMPAD
};

enum custom_keycodes {
  RGBRST = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_EESCHEMA] = LAYOUT(
        KC_ESC, KC_C, KC_A,  KC_W,
        KC_E,  KC_V,  KC_R,  KC_G,
        TO(_PCBNEW),C(KC_Z), KC_DEL, MO(_NUMPAD)
    ),
    [_PCBNEW] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D,
        KC_1, KC_2, KC_3, KC_4,
        TO(_LIBRECAD),  RGBRST, RGB_MOD, MO(_NUMPAD)
    ),
    [_LIBRECAD] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D,
        KC_1, KC_2, KC_3, KC_4,
        TO(_EESCHEMA), RGBRST, RGB_MOD, MO(_NUMPAD)
    ),
    [_NUMPAD] = LAYOUT(
        KC_7, KC_8, KC_9, KC_0,
        KC_4, KC_5, KC_6, _______,
        KC_1, KC_2, KC_3, MO(_NUMPAD)
    ),
};

extern rgblight_config_t rgblight_config;
int                      RGB_current_mode;

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
            for (int i = 0; i < 4; i++) {
        _delay_ms(50);
                rgblight_setrgb_at(RGB_CYAN, i);
            }
    for (int i = 4; i < 8; i++) {
        _delay_ms(50);
        rgblight_setrgb_at(RGB_PURPLE, i);
    }
    for (int i = 8; i < 12; i++) {
        _delay_ms(50);
        rgblight_setrgb_at(RGB_GOLD, i);
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _EESCHEMA:
            for (int i = 0; i < 4; i++) {
                rgblight_setrgb_at(RGB_CYAN, i);
            }
            break;
        case _PCBNEW:
            for (int i = 0; i < 4; i++) {
                rgblight_setrgb_at(RGB_YELLOW, i);
            }
            break;
        case _LIBRECAD:
            for (int i = 0; i < 4; i++) {
                rgblight_setrgb_at(RGB_RED, i);
            }
            break;
        default:  //  for any other layers, or the default layer
            for (int i = 0; i < 4; i++) {
                rgblight_setrgb_at(RGB_GREEN, i);
            }
            break;
    }
    for (int i = 4; i < 8; i++) {
        rgblight_setrgb_at(RGB_PURPLE, i);
    }
    for (int i = 8; i < 12; i++) {
        rgblight_setrgb_at(RGB_GOLD, i);
    }
    return state;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGBRST:
            if (record->event.pressed) {
                eeconfig_update_rgblight_default();
                rgblight_enable();
                RGB_current_mode = rgblight_config.mode;
            }
            break;
    }
    return true;
}

void matrix_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    RGB_current_mode = rgblight_config.mode;
#endif
}
/*
void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
    return true;
}
*/
