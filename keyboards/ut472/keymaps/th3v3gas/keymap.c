#include QMK_KEYBOARD_H

/*
//unicode WinCompose
//<https://github.com/samhocevar/wincompose>  default compose key is [KC_RALT]
//rules.mk (UNICODE_ENABLE = yes)
//config.h (#define UNICODE_KEY_WINC KC_RALT) <-- edit config.h to match.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};
*/

enum layers_keymap {
  _QWERTY = 0,
  _GAME,
  _THUMB,
//  _TMP,
  _FUNCTION,
  _CHAT
};

#define _QW _QWERTY
#define _GM _GAME
#define _TH _THUMB
//#define _TMP _TEMP
#define _FN _FUNCTION
#define _CH _CHAT

enum custom_keycodes {
  TD_CAPS,
  TD_QWSL,
  TD_ASTHGM,
  TD_SLSTH,
  TD_ESCTL,
  M_GUIL0 = SAFE_RANGE, //macros
  M_CHAT,
  M_CHENT,
  M_CHESC,
};

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
void td01_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_ASTHGM)
void td01_reset (qk_tap_dance_state_t *state, void *user_data);
void td02_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_SLSTH)
void td02_reset (qk_tap_dance_state_t *state, void *user_data);
void td03_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_ESCTL)
void td03_reset (qk_tap_dance_state_t *state, void *user_data);
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
//TD astrix, hold _THUMB, double tap _GAME (ref:TD_ASTHGM)
void td01_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_PAST); //astrix
      break;
    case SINGLE_HOLD:
      layer_on(_TH); //momentary _THUMB
      break;
    case DOUBLE_SINGLE_TAP:
      layer_move(_GM); //toggle _GAME
  }
}
void td01_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PAST); //astrix
      break;
    case SINGLE_HOLD:
      layer_off(_TH); //momentary _THUMB
      break;
    case DOUBLE_SINGLE_TAP:
      layer_move(_GM); //toggle _GAME
  }
}
//TD fslash, hold _THUMB, bslash (ref:TD_SLSTH)
void td02_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_PSLS); //fslash
      break;
    case SINGLE_HOLD:
      layer_on(_TH); //momentary _THUMB
      break;
    case DOUBLE_SINGLE_TAP:
      register_code16(KC_BSLS); //bslash
  }
}
void td02_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PSLS); //fslash
      break;
    case SINGLE_HOLD:
      layer_off(_TH); //momentary _THUMB
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_BSLS); //bslash
  }
}
//TD esc, hold ctrl, grave (ref:TD_ESCTL)
void td03_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_ESC); //esc
      break;
    case SINGLE_HOLD:
      register_code16(KC_LCTL); //momentary ctrl
      break;
    case DOUBLE_SINGLE_TAP:
      register_code16(KC_GRV); //grave
  }
}
void td03_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_ESC); //esc
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_LCTL); //momentary ctrl
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_GRV); //grave
  }
}
//TD _QW, fslash
void QWSLSH (qk_tap_dance_state_t *state, void *user_data) {
  switch(state->count){
    case 1:
      layer_move(_QW); //single tap, _QWERTY
      break;
    case 2:
      register_code16(KC_SLSH); //double tap, fslash
      unregister_code16(KC_SLSH);
    }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_TRNS, KC_CAPS),
  [TD_QWSL] = ACTION_TAP_DANCE_FN(QWSLSH),
  [TD_ASTHGM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
  [TD_SLSTH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td02_finished, td02_reset),
  [TD_ESCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td03_finished, td03_reset),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_GUIL0: //WIN key, _QWERTY
			if (record->event.pressed) {
        layer_clear();
        send_string(SS_TAP(X_LGUI));
			};
      return false;
    case M_CHAT: //game chat
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
        send_string(SS_TAP(X_Y));
			};
      return false;
    case M_CHENT: //game chat enter
			if (record->event.pressed) {
        layer_move(_GM);
        layer_off(_CH);
        send_string(SS_TAP(X_ENT));
			};
      return false;
    case M_CHESC: //game chat escape
			if (record->event.pressed) {
        layer_move(_GM);
        layer_off(_CH);
        send_string(SS_TAP(X_ESC));
			};
      return false;
  }
  return true;
};

//combos
//rules.mk (COMBO_ENABLE = yes)
//config.h (#define COMBO_COUNT 2) Change combo count as needed (#define COMBO_TERM 50) combo window ~50
enum combos {
  co_qm, //?
  co_ep //!
};
const uint16_t PROGMEM qm_combo[] = {KC_COMM, KC_DOT, COMBO_END}; //?
const uint16_t PROGMEM ep_combo[] = {KC_M, KC_COMM, COMBO_END}; //!
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES), //?
  [co_ep] = COMBO(ep_combo, KC_EXLM) //!
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _QWERTY _QW
*	,-------------------------------------------------------------------------.
*	|esCTL|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace | tap dance ESCTL
*	|-------------------------------------------------------------------------+
*	|tab_FN|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |' _FN |
*	|-------------------------------------------------------------------------+
*	| shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(!) |,(!?)|.(?) |QWsl| ent | combos (M+,)=! (,+.)=?, tap dance QWSL
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |SLSth |   space   |ASthGM|  +  |     |     |     | tap dance SLSTH, ASTHGM
*	`-------------------------------------------------------------------------'
*/
	[_QWERTY] = LAYOUT(TD(TD_ESCTL), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(_FN,KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(_FN,KC_QUOT),
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_QWSL), KC_SFTENT,
		KC_TRNS, KC_TRNS, KC_TRNS, LALT_T(KC_PMNS), TD(TD_SLSTH), KC_SPC, TD(TD_ASTHGM), KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS),

/* _GAME _GM
*	,-------------------------------------------------------------------------.
*	|     | p0  |  Q  |  W  |  E  |  R  | p7  |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      |  G  |  A  |  S  |  D  |  F  |     |_CHAT|     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |  B  |  Z  |  X  |  C  |  V  |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |           |      |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_GAME] = LAYOUT(KC_TRNS, KC_P0, KC_Q, KC_W, KC_E, KC_R, KC_P7, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_G, KC_A, KC_S, KC_D, KC_F, KC_TRNS, M_CHAT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_B, KC_Z, KC_X, KC_C, KC_V, KC_TRNS, KC_M, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/* _THUMB _TH
*	,-------------------------------------------------------------------------.
*	|     |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |left |down | up  |right|     |      |
*	|-------------------------------------------------------------------------+
*	|       |pause|     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  @  |  [   |     _     |   ]  |  +  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_THUMB] = LAYOUT(KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_BRK, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_AT, KC_LBRC, KC_UNDS, KC_RBRC, KC_EQL, KC_TRNS, KC_TRNS, KC_TRNS),

/* TEMP
*	,-------------------------------------------------------------------------.
*	|     |     |     |     |     |     |     |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |     @     |      |     |     |     |     |
*	`-------------------------------------------------------------------------'

	[_THUMB2] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_AT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
*/

/* _FUNCTION _FN
*	,-------------------------------------------------------------------------.
*	|(tsk)| F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |       | (tsk) crtl+shift+esc = task manager
*	|-------------------------------------------------------------------------+
*	| CAPS | F11 | F12 |     |(fnd)|     | n1  | n2  | n3  | n4  | n5  |PrtScr| tap dance TD_CAPS, (fnd) ctrl+alt+f = wox
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     | n6  | n7  | n8  | n9  | n0  | rgb |
*	|-------------------------------------------------------------------------+
*	|     |     |     | n1  |  n2  |    n3     |      | win |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_FUNCTION] = LAYOUT(LCTL(LSFT(KC_ESC)), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS,
		TD(TD_CAPS), KC_F11, KC_F12, KC_TRNS, LALT(LCTL(KC_F)), KC_TRNS, KC_P1, KC_P2, KC_P3, KC_P4, KC_P5, KC_PSCR,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P6, KC_P7, KC_P8, KC_P9, KC_P0, RGB_TOG,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_P1, KC_P2, KC_P3, KC_TRNS, M_GUIL0, KC_TRNS, KC_TRNS, KC_TRNS),

/* _CHAT _CH
*	,-------------------------------------------------------------------------.
*	| ESC |     |     |     |     |     |     |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     | ENT |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |          |       |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_CHAT] = LAYOUT(M_CHESC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, M_CHENT,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};
