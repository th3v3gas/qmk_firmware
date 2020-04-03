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
  _GAME_FUNCTION,
  _CHAT
};

#define _QW _QWERTY
#define _GM _GAME
#define _TH _THUMB
#define _FN _FUNCTION
#define _GFN _GAME_FUNCTION
#define _CH _CHAT

enum custom_keycodes {
  TD_F1F11,
  TD_F2F12,
  TD_CAPS,
  TD_QGWOX,
  TD_FNWIN,
  TD_CHENT,
  TD_PLDIS,
  TD_GMDIS,
  M_DSCRD = SAFE_RANGE, //macro
  M_CHESC,
  M_GCHAL,
  M_GCHTM,
  M_GCHAT,
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
void td03_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_FNWIN)
void td03_reset (qk_tap_dance_state_t *state, void *user_data);
void td05_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_CHENT)
void td05_reset (qk_tap_dance_state_t *state, void *user_data);
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
//TD _QWERTY; _GAME; wox (ref:TD_QGWOX)
void td01_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_QWERTY
      layer_clear();
      break;
    case SINGLE_HOLD: //_GAME
      layer_move(_GM);
      break;
    case DOUBLE_SINGLE_TAP: //wox
      layer_move(_QW);
      register_code16(KC_LALT);
      register_code16(KC_LCTL);
      register_code16(KC_MINS);
      layer_clear();
      layer_move(_QW);
      unregister_code16(KC_LALT);
      unregister_code16(KC_LCTL);
      unregister_code16(KC_MINS);
  }
}
void td01_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      layer_move(_QW);
      break;
    case SINGLE_HOLD:
      NULL;
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
//TD quote; _FN; win (ref:TD_FNWIN)
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
      layer_clear();
      layer_move(_QW);
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
//TD _GAME enter; enter; _CHAT (ref:TD_CHENT)
void td05_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_GAME enter
      layer_move(_GM);
      register_code16(KC_ENT);
      break;
    case SINGLE_HOLD: //enter
      register_code16(KC_ENT);
      unregister_code16(KC_ENT);
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
void td05_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_ENT);
      break;
    case SINGLE_HOLD:
      NULL;
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
//TD plus; discord (ref:TD_PLDIS)
void td07_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //plus
      register_code16(KC_PPLS);
      break;
    case SINGLE_HOLD: //plus
      register_code16(KC_PPLS);
      break;
    case DOUBLE_SINGLE_TAP: //discord
      register_code16(KC_F17);
      unregister_code16(KC_F17);
  }
}
void td07_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PPLS);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_PPLS);
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
//TD right; _CHAT, discord (ref:TD_GMDIS)
void td08_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //right
      register_code16(KC_RGHT);
      break;
    case SINGLE_HOLD: //right
      register_code16(KC_RGHT);
      break;
    case DOUBLE_SINGLE_TAP: //_CHAT discord
      layer_move(_QW);
      layer_on(_CH);
      register_code16(KC_F17);
      unregister_code16(KC_F17);
  }
}
void td08_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_RGHT);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_RGHT);
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_F1F11] = ACTION_TAP_DANCE_DOUBLE(KC_F1, KC_F11),
  [TD_F2F12] = ACTION_TAP_DANCE_DOUBLE(KC_F2, KC_F12),
  [TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(_______, KC_CAPS),
  [TD_QGWOX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
  [TD_FNWIN] = ACTION_TAP_DANCE_FN_ADVANCED_TIME (NULL, td03_finished, td03_reset, 160),
  [TD_CHENT] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td05_finished, td05_reset, 170),
  [TD_PLDIS] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td07_finished, td07_reset, 160),
  [TD_GMDIS] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td08_finished, td08_reset, 160),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_DSCRD: //_GAME Discord
			if (record->event.pressed) {
        send_string(SS_TAP(X_F17));
			};
      return false;
		case M_CHESC: //_GAME ESC
			if (record->event.pressed) {
        layer_move(_GM);
        send_string(SS_TAP(X_ESC));
			};
      return false;
		case M_GCHAL: //_CHAT Y
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
        send_string(SS_TAP(X_Y));
			};
      return false;
		case M_GCHTM: //_CHAT K
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
        send_string(SS_TAP(X_K));
			};
      return false;
		case M_GCHAT: //_CHAT K
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
			};
      return false;
  }
  return true;
};

//combos
//rules.mk (COMBO_ENABLE = yes)
//config.h (#define COMBO_COUNT 2) Change combo count as needed (#define COMBO_TERM 50) combo window ~50
enum combos {
  co_qm, //qmark
  co_ep //exlm
};
const uint16_t PROGMEM qm_combo[] = {KC_COMM, KC_DOT, COMBO_END}; //qmark
const uint16_t PROGMEM ex_combo[] = {KC_M, KC_COMM, COMBO_END}; //exlm
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES), //qmark
  [co_ep] = COMBO(ex_combo, KC_EXLM), //exlm
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _QW _QWERTY      DEFAULT LAYER
*	,-------------------------------------------------------------------------.
*	|esCTL|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|tab_FN|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |FNWIN | tap dance FNWIN
*	|-------------------------------------------------------------------------+
*	| shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(!) |,(!?)|.(?) |QGWOX|SFTEN| combos (M+,)=! (,+.)=?; tap dance QGWOX
*	|-------------------------------------------------------------------------+
*	|     |     |     |alt -|/ _TH |   space   |* _TH |PLDIS|     |     |     | tap dance PLDIS
*	`-------------------------------------------------------------------------'
*/
	[_QWERTY] = LAYOUT(CTL_T(KC_ESC),KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
		LT(_FN,KC_TAB),KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
		KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,TD(TD_QGWOX),KC_SFTENT,
		_______,_______,_______,LALT_T(KC_MINS),LT(_TH,KC_SLSH),KC_SPC,LT(_TH,KC_PAST),TD(TD_PLDIS),C(S(KC_EQL)),_______,_______),

/* _GM _GAME        DEFAULT LAYER
*	,-------------------------------------------------------------------------.
*	| esc |  T  |  Q  |  W  |  E  |  R  |  J  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|tb_GFN|  G  |  A  |  S  |  D  |  F  |  H  |GCHAL|GCHTM|GCHAT|  ;  |FNWIN | macro GCHAL, GCHTM, GCHAT; tap dance FNWIN
*	|-------------------------------------------------------------------------+
*	| shift |  B  |  Z  |  X  |  C  |  V  |  N  |  M  |  ,  |  .  |QGWOX|SFTEN|
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |  /  |   space   |* _TH |GMDIS|     |     |     | tap dance GMDIS
*	`-------------------------------------------------------------------------'
*/
	[_GAME] = LAYOUT(KC_ESC,KC_T,KC_Q,KC_W,KC_E,KC_R,KC_J,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
    MO(_GFN),KC_G,KC_A,KC_S,KC_D,KC_F,KC_H,M_GCHAL,M_GCHTM,M_GCHAT,KC_SCLN,TD(TD_FNWIN),
    KC_LSFT,KC_B,KC_Z,KC_X,KC_C,KC_V,KC_N,KC_M,KC_COMM,KC_DOT,TD(TD_QGWOX),KC_SFTENT,
    _______,_______,_______,KC_LBRC,KC_RBRC,KC_SPC,LT(_TH,KC_LEFT),TD(TD_GMDIS),_______,_______,_______),

/* _TH _THUMB
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |DELETE |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |left |down | up  |right|     |  \|  |
*	|-------------------------------------------------------------------------+
*	|       |undo | cut |copy |paste|     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  @  |  [   |     _     |  ]   |  =  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_THUMB] = LAYOUT(KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_DEL,
		_______,_______,_______,_______,_______,XXXXXXX,KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,_______,KC_BSLS,
		_______,C(KC_Z),C(KC_X),C(KC_C),C(KC_V),_______,_______,_______,_______,_______,TD(TD_QGWOX),_______,
		_______,_______,_______,KC_AT,KC_LBRC,KC_UNDS,KC_RBRC,KC_EQL,_______,_______,_______),

/* _FN _FUNCTION
*	,-------------------------------------------------------------------------.
*	|(tsk)|F1F11|F2F12| F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |PrtScr | (tsk) = task manager; tap dance F1F11, F2F12
*	|-------------------------------------------------------------------------+
*	| CAPS | n1  | n2  | n3  | n4  | n5  | n6  | n7  | n8  | n9  | n0  |      | tap dance TD_CAPS
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |home |pgup |pgdn | end |     |rsft |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  @  |  [   |     _     |  ]   |  =  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_FUNCTION] = LAYOUT(C(S(KC_ESC)),TD(TD_F1F11),TD(TD_F2F12),KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_PSCR,
		TD(TD_CAPS),KC_P1,KC_P2,KC_P3,KC_P4,KC_P5,KC_P6,KC_P7,KC_P8,KC_P9,KC_P0,_______,
		_______,_______,_______,_______,_______,_______,KC_HOME,KC_PGDN,KC_PGUP, KC_END,TD(TD_QGWOX),KC_RSFT,
		_______,_______,_______,KC_AT,KC_LBRC,KC_UNDS,KC_RBRC,KC_EQL,_______,_______,_______),

/*    mouse reserved
    |___| 5 | 6 | scrl|
    | 3 |pdn| 4 | f7  |
    | 1 |pup| 2 | f8  |   */

/* _GFN _GAME_FUNCTION
*	,-------------------------------------------------------------------------.
*	| del |  f1  |  7  |  8  |  9  | f4  |  Y  |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      | f2  |  4  |  5  |  6  | f5  |     |  J  |  K  |  L  |     |      |
*	|------------------------------------------------------------------------+
*	|  tab  | f3  |  1  |  2  |  3  | f6  |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	| ctl |     |     | n/  |   0  |    n*     | F11  | F12 |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_GAME_FUNCTION] = LAYOUT(
     KC_DEL,  KC_F1,   KC_7,   KC_8,   KC_9,  KC_F4,   KC_Y,_______,_______,_______,_______,_______,
    _______,  KC_F2,   KC_4,   KC_5,   KC_6,  KC_F5,KC_PPLS,   KC_J,   KC_K,   KC_L,_______,_______,
     KC_TAB,  KC_F3,   KC_1,   KC_2,   KC_3,  KC_F6,KC_PMNS,_______,_______,_______,TD(TD_QGWOX),_______,
   KC_RCTRL,_______,_______,KC_PSLS,   KC_0,KC_PAST, KC_F11, KC_F12,_______,_______,_______),

// _CH _CHAT
	[_CHAT] = LAYOUT(
    M_CHESC,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,TD(TD_QGWOX),TD(TD_CHENT),
		_______,_______,_______,_______,_______,_______,_______,M_DSCRD,_______,_______,_______),
};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}
void matrix_init_user(void) {
  rgblight_enable();
}
void matrix_scan_user(void) {
  #ifdef RGBLIGHT_ENABLE
  static uint8_t old_layer = 255;
  uint8_t new_layer = biton32(layer_state);
  if (old_layer != new_layer) {
    switch (new_layer) {
      case _QW:
        rgblight_disable_noeeprom();
        break;
      case _GM:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_MAGENTA);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 0);
        break;
      case _TH:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_CYAN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 1);
        break;
      case _FN:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_CYAN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 1);
        break;
      case _GFN:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_GREEN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 1);
        break;
      case _CH:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 2);
        break;
    }
    old_layer = new_layer;
  }
  #endif //RGBLIGHT_ENABLE
}
