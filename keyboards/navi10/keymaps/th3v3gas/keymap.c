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
    TD_STEAM,
    TD_UPLAY,
    M_VOL = SAFE_RANGE,
    M_MICMUTE,
    M_MICOPEN,
    M_MICTOG,
    M_MMDISC,
};

#define INDICATOR_LED   B5

//tap dance
//rules.mk (TAP_DANCE_ENABLE = yes)
//config.h (#define TAPPING_TERM 150) double tap window 150-200
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;
static td_state_t td_state;
int cur_dance (qk_tap_dance_state_t *state);
void td01_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_STEAM)
void td01_reset (qk_tap_dance_state_t *state, void *user_data);
void td03_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_UPLAY)
void td03_reset (qk_tap_dance_state_t *state, void *user_data);
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
// (ref:TD_STEAM)
void td01_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //
      NULL;
      break;
    case SINGLE_HOLD: //
      register_code16(KC_LSFT);
      register_code16(KC_F1);
      unregister_code16(KC_F1);
      unregister_code16(KC_LSFT);
      break;
    case DOUBLE_SINGLE_TAP: //
      register_code16(KC_LSFT);
      register_code16(KC_F1);
  }
}
void td01_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      NULL;
      break;
    case SINGLE_HOLD:
      register_code16(KC_LSFT);
      register_code16(KC_F1);
      unregister_code16(KC_F1);
      unregister_code16(KC_LSFT);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_F1);
      unregister_code16(KC_LSFT);
  }
}
// (ref:TD_UPLAY)
void td03_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //
      NULL;
      break;
    case SINGLE_HOLD: //
      register_code16(KC_LSFT);
      register_code16(KC_F2);
      unregister_code16(KC_F2);
      unregister_code16(KC_LSFT);
      break;
    case DOUBLE_SINGLE_TAP: //
      register_code16(KC_LSFT);
      register_code16(KC_F2);
  }
}
void td03_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      NULL;
      break;
    case SINGLE_HOLD:
      register_code16(KC_LSFT);
      register_code16(KC_F2);
      unregister_code16(KC_F2);
      unregister_code16(KC_LSFT);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_F2);
      unregister_code16(KC_LSFT);
  }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_STEAM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
  [TD_UPLAY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td03_finished, td03_reset),
};

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
                TD(TD_STEAM),TD(TD_UPLAY),KC_TAB,

                            M_VOL,
                 M_MMDISC,  M_MICOPEN,  M_MICMUTE),
    [_VOL] = LAYOUT(/* function layer */
                 KC_VOLD,  KC_TRNS,    KC_VOLU,
                 KC_TRNS,  KC_TRNS,    KC_TRNS,

                            KC_TRNS,
                 KC_TRNS,   KC_TRNS,    KC_TRNS),
};
