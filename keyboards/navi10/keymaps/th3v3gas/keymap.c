/* Copyright 2019 Ethan Durrant (emdarcher)
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

enum layers_keymap {
  _TOP = 0,
  _VOL,
};

enum custom_keycodes {
    M_VOL = SAFE_RANGE,
    M_MICMUTE,
    M_MICOPEN,
    M_MICTOG,
    M_MMDISC,
};

#define INDICATOR_LED   B5

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_VOL: //ear trumpet
			if (record->event.pressed) {
        register_code(KC_RCTL);
        send_string(SS_TAP(X_F23));
        unregister_code(KC_RCTL);
			};
      return false;
		case M_MICMUTE: //mute mic
			if (record->event.pressed) {
        register_code(KC_RSFT);
        send_string(SS_TAP(X_F22));
        unregister_code(KC_RSFT);
        writePinHigh(INDICATOR_LED);
			};
      return false;
		case M_MICOPEN: //open mic
			if (record->event.pressed) {
        unregister_code(KC_F22);
        register_code(KC_RCTL);
        send_string(SS_TAP(X_F22));
        unregister_code(KC_RCTL);
        writePinLow(INDICATOR_LED);
			};
      return false;
		case M_MICTOG: //toggle mic
			if (record->event.pressed) {
        register_code(KC_RSFT);
        send_string(SS_TAP(X_F23));
        unregister_code(KC_RSFT);
			};
      return false;
		case M_MMDISC: //discord mute mic
			if (record->event.pressed) {
        unregister_code(KC_F22);
        register_code(KC_F22);
			};
      return false;
  }
  return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_TOP] = LAYOUT(/* Base */
                 KC_LGUI,   MO(_VOL),   KC_ESC,
                 S(KC_F1),  S(KC_F2),   KC_TAB,

                            M_VOL,
                 M_MMDISC,  M_MICOPEN,  M_MICMUTE),
    [_VOL] = LAYOUT(/* function layer */
                 KC_VOLD,  KC_TRNS,    KC_VOLU,
                 KC_TRNS,  KC_TRNS,    KC_TRNS,

                            KC_TRNS,
                 KC_TRNS,   KC_TRNS,    KC_TRNS),
};
