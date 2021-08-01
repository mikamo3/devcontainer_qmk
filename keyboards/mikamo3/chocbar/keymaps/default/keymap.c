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
    _BASE,
    _LOWER,
    _RAISE
};

enum { TD_ESCGUI = 0 };
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KC_Q, KC_W, KC_E, KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,
        KC_A, KC_S, KC_D, KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,
        KC_LCTL, KC_Z, KC_X, KC_C, KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,
        KC_LSFT,KC_ESC,KC_TAB,KC_LGUI,LT(_LOWER,KC_SPC),LT(_RAISE,KC_ENT),KC_RGUI,KC_RALT,KC_BSPC
    ),
    [_LOWER] = LAYOUT(
        KC_EXLM,KC_AT,KC_HASH,KC_DLR,KC_PERC,KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,
        KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,
        _______,KC_EQL, KC_DQT, KC_LCBR, KC_LBRC, KC_MINS, KC_RBRC, KC_RCBR, KC_QUOT,KC_GRV,KC_BSLS,
        _______,A(KC_GRV),_______,_______,_______,_______,_______,_______,KC_DEL
    ),
    [_RAISE] = LAYOUT(
   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                  KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,
        KC_F11, KC_HOME, KC_PGDN, KC_PGUP,  KC_END,                KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_F12,
   RGB_TOG,_______, KC_PSCR,  KC_INS, KC_SLCK, KC_PAUS,                _______,_______,_______,_______,_______,
        RGB_MOD,_______,_______,_______,_______,_______,_______,_______,KC_DEL
    )
};

void dance_cln_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_on(_RAISE);
    } else {
        register_code(KC_LGUI);
    }
}

void dance_cln_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_off(_RAISE);
    } else{
        unregister_code(KC_LGUI);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {[TD_ESCGUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_cln_finished, dance_cln_reset)};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    }
    return true;
}
void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
      rgblight_mode(7);
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
