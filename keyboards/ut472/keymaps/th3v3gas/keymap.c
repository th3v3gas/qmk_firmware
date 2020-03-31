#include QMK_KEYBOARD_H

/*
//unicode WinCompose
//<https://github.com/samhocevar/wincompose>
//rules.mk (UNICODE_ENABLE = yes)                 |default compose key is [KC_RALT]|
//config.h (#define UNICODE_KEY_WINC KC_RALT) <-- |edit config.h to match.         |
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};
*/

enum layers_keymap {
  _QWERTY = 0,
  _GAME,
  _THUMB,
  _FUNCTION,
  _CHAT
};

#define _QW _QWERTY
#define _GM _GAME
#define _TH _THUMB
#define _FN _FUNCTION
#define _CH _CHAT

enum custom_keycodes {
  TD_CAPS,
  TD_QGWOX,
  TD_GMWIN,
  TD_FNWIN,
  TD_CHESC,
  TD_CHENT,
  TD_CHJ,
  TD_CHK,
  TD_PLDIS,
  TD_GMDIS,
  M_DSCRD = SAFE_RANGE, //macro
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
void td01_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_QGWOX)
void td01_reset (qk_tap_dance_state_t *state, void *user_data);
void td02_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_GMWIN)
void td02_reset (qk_tap_dance_state_t *state, void *user_data);
void td03_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_FNWIN)
void td03_reset (qk_tap_dance_state_t *state, void *user_data);
void td04_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_CHESC)
void td04_reset (qk_tap_dance_state_t *state, void *user_data);
void td05_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_CHENT)
void td05_reset (qk_tap_dance_state_t *state, void *user_data);
void td06_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_CHJ)
void td05_reset (qk_tap_dance_state_t *state, void *user_data);
void td09_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_CHK)
void td09_reset (qk_tap_dance_state_t *state, void *user_data);
void td07_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_PLDIS)
void td07_reset (qk_tap_dance_state_t *state, void *user_data);
void td08_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_GMDIS)
void td08_reset (qk_tap_dance_state_t *state, void *user_data);
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
//TD _QWERTY, _GAME, wox (ref:TD_QGWOX)
void td01_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_QWERTY
      layer_clear();
      break;
    case SINGLE_HOLD: //_GAME
      layer_move(_QW);
      break;
    case DOUBLE_SINGLE_TAP: //wox
      register_code16(KC_LALT);
      register_code16(KC_LCTL);
      register_code16(KC_F);
  }
}
void td01_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      layer_move(_QW);
      break;
    case SINGLE_HOLD:
      layer_on(_GM);
      break;
    case DOUBLE_SINGLE_TAP:
      layer_clear();
      layer_move(_QW);
      unregister_code16(KC_LALT);
      unregister_code16(KC_LCTL);
      unregister_code16(KC_F);
  }
}
//TD quote, _FUNCTION, _CHAT win (ref:TD_GMWIN)
void td02_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //quote
      register_code16(KC_QUOT);
      break;
    case SINGLE_HOLD: //momentary _FUNCTION
      layer_move(_QW);
      layer_on(_FN);
      break;
    case DOUBLE_SINGLE_TAP: //_CHAT win
      layer_move(_QW);
  }
}
void td02_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_QUOT);
      break;
    case SINGLE_HOLD:
      layer_move(_QW);
      layer_on(_GM);
      break;
    case DOUBLE_SINGLE_TAP:
      register_code16(KC_LGUI);
      _delay_ms(10);
      unregister_code16(KC_LGUI);
  }
}
//TD quote, _FN, win (ref:TD_FNWIN)
void td03_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //quote
      register_code16(KC_QUOT);
      break;
    case SINGLE_HOLD: //momentary _FM
      layer_on(_FN);
      break;
    case DOUBLE_SINGLE_TAP: //win
      register_code16(KC_LGUI);
  }
}
void td03_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_QUOT);
      break;
    case SINGLE_HOLD:
      layer_off(_FN);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_LGUI);
  }
}
//TD _GAME esc, esc (ref:TD_CHESC)
void td04_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_GAME esc
      register_code16(KC_ESC);
      break;
    case SINGLE_HOLD: //esc
      unregister_code16(KC_ESC);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_ESC);
  }
}
void td04_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_ESC);
      layer_move(_QW);
      layer_on(_GM);
      break;
    case SINGLE_HOLD:
      register_code16(KC_ESC);
      _delay_ms(10);
      unregister_code16(KC_ESC);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_ESC);
  }
}
//TD _GAME enter, enter (ref:TD_CHENT)
void td05_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_GAME enter
      register_code16(KC_ENT);
      break;
    case SINGLE_HOLD: //enter
      unregister_code16(KC_ENT);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_ENT);
  }
}
void td05_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_ENT);
      layer_move(_QW);
      layer_on(_GM);
      break;
    case SINGLE_HOLD:
      register_code16(KC_ENT);
      _delay_ms(10);
      unregister_code16(KC_ENT);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_ENT);
  }
}
//_CHAT Y, J (ref:TD_CHJ)
void td06_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_CHAT Y
      register_code16(KC_Y);
      break;
    case SINGLE_HOLD: //J
      register_code16(KC_J);
      break;
    case DOUBLE_SINGLE_TAP: //J
      unregister_code16(KC_J);
  }
}
void td06_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_Y);
      layer_move(_QW);
      layer_on(_CH);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_J);
      _delay_ms(10);
      unregister_code16(KC_J);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_J);
  }
}
//TD K _CHAT, K(ref:TD_CHK)
void td09_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //K
      register_code16(KC_K);
      break;
    case SINGLE_HOLD: //_CHAT Y
      unregister_code16(KC_J);
      break;
    case DOUBLE_SINGLE_TAP: //J
      unregister_code16(KC_K);
  }
}
void td09_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_K);
      layer_move(_QW);
      layer_on(_CH);
      break;
    case SINGLE_HOLD:
      register_code16(KC_K);
      _delay_ms(10);
      unregister_code16(KC_K);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_K);
  }
}
//TD plus, discord (ref:TD_PLDIS)
void td07_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //plus
      register_code16(KC_PAST);
      break;
    case SINGLE_HOLD: //plus
      register_code16(KC_PAST);
      break;
    case DOUBLE_SINGLE_TAP: //discord
      register_code16(KC_F13);
  }
}
void td07_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PAST);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_PAST);
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_F13);
  }
}
//TD plus, _CHAT discord (ref:TD_GMDIS)
void td08_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //plus
      register_code16(KC_PAST);
      break;
    case SINGLE_HOLD: //plus
      register_code16(KC_PAST);
      break;
    case DOUBLE_SINGLE_TAP: //_CHAT discord
      register_code16(KC_F13);
  }
}
void td08_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PAST);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_PAST);
      break;
    case DOUBLE_SINGLE_TAP:
      layer_move(_QW);
      layer_on(_CH);
      unregister_code16(KC_F13);
  }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_TRNS, KC_CAPS),
  [TD_QGWOX] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td01_finished, td01_reset, 160),
  [TD_GMWIN] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td02_finished, td02_reset, 160),
  [TD_FNWIN] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td03_finished, td03_reset, 160),
  [TD_CHESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td04_finished, td04_reset),
  [TD_CHENT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td05_finished, td05_reset),
  [TD_CHJ] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td06_finished, td06_reset),
  [TD_CHK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td09_finished, td09_reset),
  [TD_PLDIS] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td07_finished, td07_reset, 160),
  [TD_GMDIS] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td08_finished, td08_reset, 160),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_DSCRD:
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_GM);
        send_string(SS_TAP(X_F13));
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
const uint16_t PROGMEM qm_combo[] = {KC_COMM, KC_DOT, COMBO_END}; //qmark
const uint16_t PROGMEM ex_combo[] = {KC_M, KC_COMM, COMBO_END}; //exlm
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES), //qmark
  [co_ep] = COMBO(ex_combo, KC_EXLM), //exlm
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _QWERTY _QW
*	,-------------------------------------------------------------------------.
*	|esCTL|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|tab_FN|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |FNWIN | tap dance FNWIN
*	|-------------------------------------------------------------------------+
*	| shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(!) |,(!?)|.(?) |QGWOX| ent | combos (M+,)=! (,+.)=?, tap dance QGWOX
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |/ _TH |   space   |* _TH |PLDIS|     |     |     | tap dance PLDIS
*	`-------------------------------------------------------------------------'
*/
	[_QWERTY] = LAYOUT(CTL_T(KC_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(_FN,KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, TD(TD_FNWIN),
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_QGWOX), KC_SFTENT,
		KC_TRNS, KC_TRNS, KC_TRNS, LALT_T(KC_PMNS), LT(_TH,KC_PSLS), KC_SPC, LT(_TH,KC_PAST), TD(TD_PLDIS), KC_TRNS, KC_TRNS, KC_TRNS),

/* _GAME _GM
*	,-------------------------------------------------------------------------.
*	| esc | T  |  Q  |  W  |  E  |  R  | F10 |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|  tab  |  G  |  A  |  S  |  D  |  F  |     | CHJ | CHK |     |     |GMWIN| tap dance CHJ, CHK, GMWIN
*	|-------------------------------------------------------------------------+
*	| shift |  B  |  Z  |  X  |  C  |  V  |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |  /   |   space   |      |GMDIS|     |     |     | tap dance GMDIS
*	`-------------------------------------------------------------------------'
*/
	[_GAME] = LAYOUT(KC_ESC, KC_T, KC_Q, KC_W, KC_E, KC_R, KC_F10, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_G, KC_A, KC_S, KC_D, KC_F, KC_TRNS, TD(TD_CHJ), TD(TD_CHK), KC_TRNS, KC_TRNS, TD(TD_GMWIN),
		KC_TRNS, KC_B, KC_Z, KC_X, KC_C, KC_V, KC_TRNS, KC_M, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_PMNS, KC_PSLS, KC_TRNS, KC_TRNS, TD(TD_GMDIS), KC_TRNS, KC_TRNS, KC_TRNS),

/* _THUMB _TH
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |DELETE |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |left |down | up  |right|     |  \|  |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     | /?  |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  @  |  [   |     _     |   ]  |  =  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_THUMB] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_DEL,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_BSLS,
		KC_TRNS, RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SLSH, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_AT, KC_LBRC, KC_UNDS, KC_RBRC, KC_EQL, KC_TRNS, KC_TRNS, KC_TRNS),

/* _FUNCTION _FN
*	,-------------------------------------------------------------------------.
*	|(tsk)| F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |       | (tsk) crtl+shift+esc = task manager
*	|-------------------------------------------------------------------------+
*	| CAPS | F11 | F12 |     |     |     | n1  | n2  | n3  | n4  | n5  |PrtScr| tap dance TD_CAPS, (fnd) ctrl+alt+f = wox
*	|-------------------------------------------------------------------------+
*	|       |pause|     |     |     |     | n6  | n7  | n8  | n9  | n0  | rgb |
*	|-------------------------------------------------------------------------+
*	|     |     |     | n1  |  n2  |    n3     |      | rgb |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_FUNCTION] = LAYOUT(C(S(KC_ESC)), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS,
		TD(TD_CAPS), KC_F11, KC_F12, KC_TRNS, KC_TRNS, KC_TRNS, KC_P1, KC_P2, KC_P3, KC_P4, KC_P5, KC_PSCR,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P6, KC_P7, KC_P8, KC_P9, KC_P0, RGB_TOG,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_P1, KC_P2, KC_P3, KC_TRNS, RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS),

/* _CHAT _CH
*	,-------------------------------------------------------------------------.
*	|CHESC|     |     |     |     |     |     |     |     |     |     |       | tap dance TD_CHESC
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |    |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |CHENT| tap dance TD_CHENT
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |          |       |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_CHAT] = LAYOUT(TD(TD_CHESC), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TD(TD_CHENT),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, M_DSCRD, KC_TRNS, KC_TRNS, KC_TRNS),
};
