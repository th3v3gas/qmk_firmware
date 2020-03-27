#include QMK_KEYBOARD_H

//unicode WinCompose
//<https://github.com/samhocevar/wincompose>  default compose key is [KC_RALT]
//rules.mk (UNICODE_ENABLE = yes)
//config.h (#define UNICODE_KEY_WINC KC_RALT) <-- edit config.h to match.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};

//tap dance
//rules.mk (TAP_DANCE_ENABLE = yes)
//config.h (#define TAPPING_TERM 170) double tap window 150-200
enum custom_keycodes {TD_LBRC,
  TD_RBRC,
  TD_CAPS,
  TD_NP1,
  TD_NP2,
  TD_NP3,
  TD_NP4,
  TD_EMO1,
  TD_DHO,
  TD_UEN
};
// ¯\_(ツ)_/¯ on tap, UwU on double tap
void TDEMO1 (qk_tap_dance_state_t *state, void *user_data) {
  switch(state->count){
    case 1:
      send_unicode_string("¯\\_(ツ)_/¯");
      break;
    case 2:
      send_string("UwU");
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
  [TD_EMO1] = ACTION_TAP_DANCE_FN(TDEMO1),
  [TD_DHO] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, KC_HOME),
  [TD_UEN] = ACTION_TAP_DANCE_DOUBLE(KC_UP, KC_END)
};

//combos
//rules.mk (COMBO_ENABLE = yes)
//config.h (#define COMBO_COUNT 2) Change combo count as needed (#define COMBO_TERM 50) combo window ~50
enum combos {
  co_qm,
  co_ep
};
const uint16_t PROGMEM qm_combo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM ep_combo[] = {KC_M, KC_COMM, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES),
  [co_ep] = COMBO(ep_combo, KC_EXLM)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*	Base Layer
*	,-------------------------------------------------------------------------.
*	|(esc)|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace | (ctrl+alt)/esc
*	|-------------------------------------------------------------------------+
*	|Tab/L2|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  | '/L3 |
*	|-------------------------------------------------------------------------+
*	| Shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(?) |,(?!)|.(!) |emote| Ent | combo (m+,)=! combo (,+.)=? tap dance emote ¯\_(ツ)_/¯, UwU
*	|-------------------------------------------------------------------------+
*	| Ctrl|  _  |=/Alt|  /  |   *  |   Space   |Del/L4| left|v(HO)|^(EN)|Right| tap dance down(home) up(end)
*	`-------------------------------------------------------------------------'
*/
	[0] = LAYOUT(LCA_T(KC_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(2,KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(3,KC_QUOT),
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_EMO1), KC_SFTENT,
		KC_LCTL, KC_UNDS, LALT_T(KC_EQL), KC_PSLS, KC_PAST, KC_SPC, LT(4,KC_DEL), KC_LEFT, TD(TD_DHO), TD(TD_UEN), KC_RIGHT),

/*	Game Layer
*	,-------------------------------------------------------------------------.
*	|(esc)|  Q  |  -  |  W  |  E  |  R  |  +  |  U  |  I  |  O  |  P  |Bspace | (ctrl+alt)/esc
*	|-------------------------------------------------------------------------+
*	|Tab/L2|  F  |  A  |  S  |  D  |  G  |  H  |  J  |  K  |  L  |  ;  | '/L3 |
*	|-------------------------------------------------------------------------+
*	| Shift |  V  |  Z  |  X  |  C  |  B  |  N  |M(?) |,(?!)|.(!) |emote| L0  | combo (m+,)=! combo (,+.)=? tap dance emote ¯\_(ツ)_/¯, UwU
*	|-------------------------------------------------------------------------+
*	| Ctrl|  _  |=/Alt|  /  |   *  |   Space   |Del/L4| left|v(HO)|^(EN)|Right| tap dance down(home) up(end)
*	`-------------------------------------------------------------------------'
*/
	[1] = LAYOUT(LCA_T(KC_ESC), KC_Q, KC_PMNS, KC_W, KC_E, KC_R, KC_PPLS, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(2,KC_TAB), KC_F, KC_A, KC_S, KC_D, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(3,KC_QUOT),
		KC_LSFT, KC_V, KC_Z, KC_X, KC_C, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_EMO1), DF(0),
		KC_LCTL, KC_UNDS, LALT_T(KC_EQL), KC_PSLS, KC_PAST, KC_SPC, LT(4,KC_DEL), KC_LEFT, TD(TD_DHO), TD(TD_UEN), KC_RIGHT),

/*
*	Layer 2 tab
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |9([) |0(]) |       | tap dance [ ]
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |  4  |     |     |     |  \   |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       | Home| PgDn| PgUp| End |
*	`-------------------------------------------------------------------------'
*/
	[2] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSLS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END),
/*
*	Layer 3 '
*	,-------------------------------------------------------------------------.
*	| ` ~ |(np)1|(np)2|(np)3|(np)4|  5  |  6  |  7  |  8  |9([) |0(]) |       | tap dance NumPad 1-4 [ ]
*	|-------------------------------------------------------------------------+
*	|      |  n5  | n6  | n7 |  n8 |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       | n9  | n0  |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       |     | L1  |rCtl | Gui |
*	`-------------------------------------------------------------------------'
*/
	[3] = LAYOUT(KC_GRV, TD(TD_NP1), TD(TD_NP2), TD(TD_NP3), TD(TD_NP4), KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_P5, KC_P6, KC_P7, KC_P8, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_P9, KC_P0, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, DF(1), KC_RCTL, KC_RGUI),
/*
*	Layer 4 Del
*	,-------------------------------------------------------------------------.
*	| Esc | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |PrtScr |
*	|-------------------------------------------------------------------------+
*	| (CL) | F11 | F12 |     |     |     |     |     |     |RGB- |RGB+ |      | tap dance CapLock
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     | Br- | Br+ |RGBt |
*	|-------------------------------------------------------------------------+
*	|     |Pause|     |     |      |          |       |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[4] = LAYOUT(KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_PSCR,
		TD(TD_CAPS), KC_F11, KC_F12, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_RMOD, RGB_MOD, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_VAD, RGB_VAI, RGB_TOG,
		KC_TRNS, KC_PAUS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};
