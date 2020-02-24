#include QMK_KEYBOARD_H

//unicode WinCompose rules.mk (UNICODE_ENABLE = yes) config.h (#define UNICODE_KEY_WINC KC_RALT)
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};

//tap dance rules.mk (TAP_DANCE_ENABLE = yes)
enum {
	TD_LBRC,
	TD_RBRC,
	TD_CAPS,
	TD_NP1,
	TD_NP2,
	TD_NP3,
	TD_NP4,
	TD_EMO,
};
void TDEMO (qk_tap_dance_state_t *state, void *user_data) {
	switch(state->count){
		case 1:
			send_unicode_hex_string("00AF 005C 005F 0028 30C4 0029 005F 002F 00AF");
			break;
		case 2:
			send_string(":]");
			break;
    default:
	break;
	}
}
qk_tap_dance_action_t tap_dance_actions[] = {
	[TD_LBRC] = ACTION_TAP_DANCE_DOUBLE(KC_9, KC_LBRC),
	[TD_RBRC] = ACTION_TAP_DANCE_DOUBLE(KC_0, KC_RBRC),
	[TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_TRNS, KC_CAPS),
	[TD_NP1] = ACTION_TAP_DANCE_DOUBLE(KC_1, KC_P1),
	[TD_NP2] = ACTION_TAP_DANCE_DOUBLE(KC_2, KC_P2),
	[TD_NP3] = ACTION_TAP_DANCE_DOUBLE(KC_3, KC_P3),
	[TD_NP4] = ACTION_TAP_DANCE_DOUBLE(KC_4, KC_P4),
	[TD_EMO] = ACTION_TAP_DANCE_FN(TDEMO),
};

//combos, rules.mk (COMBO_ENABLE = yes) config.h (#define COMBO_COUNT 2) Change combo count as needed
enum combos {
	co_qm,
	co_ep
};
const uint16_t PROGMEM ep_combo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM qm_combo[] = {KC_M, KC_COMM, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES),
	[co_ep] = COMBO(ep_combo, KC_EXLM)
};
/*
//macros
enum custom_keycodes{
	SHRUG = SAFE_RANGE,
	SMILE,
};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case SHRUG:
			if (record->event.pressed) {
				send_unicode_hex_string("00AF 005C 005F 0028 30C4 0029 005F 002F 00AF");
			} else {
			}
			break;
		case SMILE:
			if (record->event.pressed) {
				send_string(":]");
			} else {
			}
			break;
	}
	return true;
};
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*	Base Layer
*	,-------------------------------------------------------------------------.
*	| /Esc|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace | (Ctrl+Alt)/esc
*	|-------------------------------------------------------------------------+
*	|Tab/L3|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |   '  |
*	|-------------------------------------------------------------------------+
*	| Shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(?) |,(?!)|.(!) |emote| Ent | Comb(m+,)=? Comb(,+.)=!
*	|-------------------------------------------------------------------------+
*	| Ctrl|  _  |=/Alt|  /  |   *  |   Space   |  Del | Left| Down|  Up |Right|
*	`-------------------------------------------------------------------------'
*/
	[0] = LAYOUT(LCA_T(KC_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(1,KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(2,KC_QUOT),
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_EMO), KC_SFTENT,
		KC_LCTL, KC_UNDS, LALT_T(KC_EQL), KC_PSLS, KC_PAST, KC_SPC, LT(3,KC_DEL), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),
/*
*	Layer 1 tab
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |9([) |0(]) |       | Tap dance [ ]
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |     |     |     |     |  \   |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       | Home| PgDn| PgUp| End |
*	`-------------------------------------------------------------------------'
*/
	[1] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSLS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TD(TD_EMO), KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END),
/*
*	Layer 2 '
*	,-------------------------------------------------------------------------.
*	| ` ~ |(np)1|(np)2|(np)3|(np)4|  5  |  6  |  7  |  8  |9([) |0(]) |       | Tap dance Numpad 1-4 [ ]
*	|-------------------------------------------------------------------------+
*	|      |  N5  | N6  | N7 |  N8 |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       | N9  | N0  |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       |     |     |rCtl | Gui |
*	`-------------------------------------------------------------------------'
*/
	[2] = LAYOUT(KC_GRV, TD(TD_NP1), TD(TD_NP2), TD(TD_NP3), TD(TD_NP4), KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_P5, KC_P6, KC_P7, KC_P8, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_P9, KC_P0, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RCTL, KC_RGUI),
/*
*	Layer 3 Del
*	,-------------------------------------------------------------------------.
*	| Esc | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |PrtScr |
*	|-------------------------------------------------------------------------+
*	|CapLoc| F11 | F12 |     |     |     |     |     |     |RGB- |RGB+ |      | Tap dance caplock
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     | Br- | Br+ |RGBt |
*	|-------------------------------------------------------------------------+
*	|     |Pause|     |     |      |          |       |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[3] = LAYOUT(KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_PSCR,
		TD(TD_CAPS), KC_F11, KC_F12, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_RMOD, RGB_MOD, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_VAD, RGB_VAI, RGB_TOG,
		KC_TRNS, KC_PAUS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};