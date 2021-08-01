/* Copyright 2021 mikamo3
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
#include <string.h>
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#include "quantum_keycodes.h"
#include "code_pattern.h"
#include "sendstring_jis.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
};

// Defines the keycodes used by our macros in process_record_user
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(MY_1, MY_2, MY_3, MY_4, MY_5, MY_6, MY_7, MY_8, MY_9, MY_0, MY_A, MY_B)

};

#define MARIO S__NOTE(_C5), S__NOTE(_G4), S__NOTE(_C5), S__NOTE(_E5), S__NOTE(_G5), S__NOTE(_C6), S__NOTE(_G5), S__NOTE(_GS4), S__NOTE(_C5), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_C6), S__NOTE(_DS6), S__NOTE(_GS6), S__NOTE(_DS6), S__NOTE(_AS4), S__NOTE(_D5), S__NOTE(_F5), S__NOTE(_AS5), S__NOTE(_D6), S__NOTE(_F6), S__NOTE(_AS6), S__NOTE(_F6)


uint16_t input_push_pattern[PUSH_COUNT] = {0};
struct dial_pattern input_rotally_pattern[3] = {0};

uint8_t rotally_pos    = 0;
uint8_t rotally_cnt    = 0;
bool    prev_clockwise = true;

uint8_t push_pos = 0;

#ifdef AUDIO_ENABLE
float tone_plover_gb[][2] = SONG(MARIO);
#endif

void reset_push_state(void);
void reset_rotally_state(void);
void reset_state(void);

void reset_push_state(void) {
    for (uint8_t i = 0; i < PUSH_COUNT; i++) {
        input_push_pattern[i] = 0;
    }
    push_pos = 0;
}

void reset_rotally_state(void) {
    for (uint8_t i = 0; i < ROTALLY_COUNT; i++) {
        input_rotally_pattern[i].clockwise = true;
        input_rotally_pattern[i].count     = 0;
    }
    rotally_pos    = 0;
    rotally_cnt    = 0;
    prev_clockwise = true;
}

void reset_state(void) {
    reset_push_state();
    reset_rotally_state();
}

void keyboard_post_init_user(void) { rgblight_sethsv_noeeprom(0, 0, 0); }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            // check rotally
            case MY_A:
                for (uint8_t i = 0; i < ROTALLY_PATTERN_NUM; i++) {
                    for (uint8_t j = 0; j < ROTALLY_COUNT; j++) {
                        if (input_rotally_pattern[j].clockwise != ROTALLY_PATTERN[i][j].clockwise || input_rotally_pattern[j].count != ROTALLY_PATTERN[i][j].count) {
                            break;
                        }
                        // match
                        if (j == ROTALLY_COUNT - 1) {

                            rgblight_sethsv_noeeprom_green();
                            switch (i) {
                                case 0:
                                    SEND_STRING(ROTALLY_CODE_0);
                                    break;
                                default:
                                    break;
                            }
                            rgblight_sethsv_noeeprom(HSV_BLACK);
                            reset_state();
                            return true;
                        }
                    }
                }
                // wrong

                rgblight_sethsv_noeeprom(HSV_BLACK);
                reset_state();
                return true;
                break;
            case MY_B:

                rgblight_sethsv_noeeprom(HSV_BLACK);
                reset_state();
                break;

            default:
                if (rotally_pos != 0) {
                    rgblight_sethsv_noeeprom(HSV_BLACK);
                    reset_rotally_state();
                }
                input_push_pattern[push_pos] = keycode;
                if (push_pos == (PUSH_COUNT - 1)) {
                    for (uint8_t i = 0; i < PUSH_PATTERN_NUM; i++) {
                        for (uint8_t j = 0; j < PUSH_COUNT; j++) {
                            if (input_push_pattern[j] != PUSH_PATTERN[i][j]) {
                                break;
                            }

                            // match
                            if (j == PUSH_COUNT - 1) {

#ifdef AUDIO_ENABLE
                                PLAY_SONG(tone_plover_gb);
#endif

                                rgblight_sethsv_noeeprom_green();
                                switch (i) {
                                    case 0:
                                        SEND_STRING(PUSH_CODE_0);
                                        break;
                                    case 1:
                                        SEND_STRING(PUSH_CODE_1);
                                        break;
                                    case 2:
                                        SEND_STRING(PUSH_CODE_2);
                                        break;
                                    case 3:
                                        SEND_STRING(PUSH_CODE_3);
                                        break;

                                    case 4:
                                        SEND_STRING(PUSH_CODE_4);
                                        break;
                                    case 5:
                                        SEND_STRING(PUSH_CODE_5);
                                        break;
                                    case 6:
                                        SEND_STRING(PUSH_CODE_6);
                                        break;
                                    default:
                                        break;
                                }
                                SEND_STRING(SS_DELAY(1000));
                                rgblight_sethsv_noeeprom(HSV_BLACK);

                                reset_push_state();
                                return true;
                            }
                        }
                    }
                    // wrong

                                rgblight_sethsv_noeeprom(HSV_BLACK);
                    reset_push_state();
                    return true;
                } else {
                    rgblight_sethsv_white_at(push_pos);
                    push_pos++;
                    return true;
                }
                break;
        }
    }
    return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t led_pos;
    uint8_t led_color;
    if (rotally_pos == ROTALLY_COUNT || rotally_cnt == 0xff) {
        rgblight_sethsv_noeeprom(HSV_BLACK);
        reset_rotally_state();
    }
    if (rotally_pos != 0 || rotally_cnt != 0) {
        if (prev_clockwise != clockwise) {
            rgblight_sethsv_noeeprom(HSV_BLACK);
            rotally_pos++;
            rotally_cnt = 0;
        }
    }
    prev_clockwise = clockwise;
    rotally_cnt++;
    if (clockwise) {
        led_pos = (rotally_cnt - 1) % RGBLED_NUM;

    } else {
        led_pos = (RGBLED_NUM - 1) - (rotally_cnt - 1) % RGBLED_NUM;
    }
    led_color = (rotally_cnt - 1) / RGBLED_NUM % 3;
    switch (led_color) {
        case 0:

            rgblight_sethsv_at(HSV_RED, led_pos);
            break;

        case 1:

            rgblight_sethsv_at(HSV_GREEN, led_pos);
            break;

        case 2:

            rgblight_sethsv_at(HSV_BLUE, led_pos);
            break;
        default:
            break;
    }
    input_rotally_pattern[rotally_pos].clockwise = clockwise;
    input_rotally_pattern[rotally_pos].count     = rotally_cnt;
}
