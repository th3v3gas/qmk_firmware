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
  _TOP00 = 0,
  _QWERTY,
  _TOP01,
  _FUNCTION,
  _QWERTY_TH,
  _CHAT
};

#define _TP0 _TOP00
#define _QTY _QWERTY
#define _TP1 _TOP01
#define _FN _FUNCTION
#define _QYT _QWERTY_TH
#define _CH _CHAT

enum custom_keycodes {
  TD_F1F11,
  TD_F2F12,
  TD_QGWOX,
  //TD_tmp82, free macro
  M_AFKTG = SAFE_RANGE, //OBS afk toggle
  M_GAME0,  //OBS scene: game -cam
  M_GAME1,  //OBS scene: game
  M_CHAT0,  //OBS scene: chat -cam
  M_CHAT1,  //OBS scene: chat
  M_CAP,    //OBS scene: screen capture
  M_PRVT1,  //OBS mute mic, camera off
  M_VZIO0,  //OBS mute vizio
  M_MIC0,   //OBS mic off
  M_MIC1,   //OBS mic on
  M_VZIO1,  //OBS unmute vizio
  M_ALRT0,  //OBS alerts off
  M_ALRT1,  //OBS alerts on
  M_RPLAY,  //OBS save save replay
  M_SBRA0,  //OBS mute siberia
  M_SBRA1,  //OBS unmute siberia
  M_SMSNG0, //OBS mute samsung
  M_SMSNG1, //OBS unmute samsung
  M_PRVW0,  //OBE preview off
  M_VOIP0,  //OBS mute voip
  M_VOIP1,  //OBS unmute voip
  M_PRVW1,  //OBE preview on
  M_BRB,    //OBS scene: brb
  M_TMP23,
  M_STRT0,  //OBS scene: start
  M_TMP25,
  M_IRPLY,  //OBS instant replay
  M_TMP27,
  M_TMP28,
  M_TMP29,
  M_TMP30,
  M_TMP31,
  M_TMP32,
  M_TMP33,
  M_TRMPT,  //ear trumpet
  M_MICOPN, //mic on  GLOBAL
  M_MICCLS, //mic off GLOBAL
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
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
//(ref:TD_QGWOX)
void td01_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_TOP00
      layer_clear();
      break;
    case SINGLE_HOLD: //_QWERTY
      layer_move(_QTY);
      break;
    case DOUBLE_SINGLE_TAP: //wox
      layer_clear();
      layer_move(_QTY);
      register_code16(KC_RCTL);
      register_code16(KC_F24);
      unregister_code16(KC_F24);
      unregister_code16(KC_RCTL);
  }
}
void td01_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      layer_move(_TP0);
      break;
    case SINGLE_HOLD:
      NULL;
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
//free macro(ref:TD_tmp82)
/*
void tmp82_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F14);
    } else {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F15);
    }
}
void tmp82_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code(KC_F14);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
    } else {
        unregister_code(KC_F15);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
    }
}
*/
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_F1F11] = ACTION_TAP_DANCE_DOUBLE(KC_F1, KC_F11),
  [TD_F2F12] = ACTION_TAP_DANCE_DOUBLE(KC_F2, KC_F12),
  [TD_QGWOX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
//  [TD_tmp82] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, tmp82_finished, tmp82_reset, 160),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
// SHIFT + GUI + FUNCTION
		case M_AFKTG:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F13);
       } else {
        unregister_code(KC_F13);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_GAME0:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F14);
       } else {
        unregister_code(KC_F14);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_GAME1:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F15);
       } else {
        unregister_code(KC_F15);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_CHAT0:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F16);
       } else {
        unregister_code(KC_F16);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_CHAT1:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F17);
       } else {
        unregister_code(KC_F17);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_CAP:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F18);
       } else {
        unregister_code(KC_F18);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_PRVT1:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F19);
       } else {
        unregister_code(KC_F19);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_VZIO0:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F20);
       } else {
        unregister_code(KC_F20);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_MIC0:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F23);
       } else {
        unregister_code(KC_F23);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_MIC1:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LGUI);
        register_code(KC_F24);
       } else {
        unregister_code(KC_F24);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSFT);
			};
      return false;

// CTRL + ALT + FUNCTION
		case M_VZIO1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F13);
       } else {
        unregister_code(KC_F13);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_ALRT0:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F14);
       } else {
        unregister_code(KC_F14);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_ALRT1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F15);
       } else {
        unregister_code(KC_F15);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_RPLAY:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F16);
       } else {
        unregister_code(KC_F16);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_SBRA0:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F17);
       } else {
        unregister_code(KC_F17);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_SBRA1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F18);
       } else {
        unregister_code(KC_F18);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_SMSNG0:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F19);
       } else {
        unregister_code(KC_F19);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_SMSNG1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F20);
       } else {
        unregister_code(KC_F20);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_PRVW0:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F21);
       } else {
        unregister_code(KC_F21);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_VOIP0:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F22);
       } else {
        unregister_code(KC_F22);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_VOIP1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F23);
       } else {
        unregister_code(KC_F23);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_PRVW1:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_F24);
       } else {
        unregister_code(KC_F24);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
			};
      return false;

// SHIFT + ALT + FUNCTION
		case M_BRB:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F13);
       } else {
        unregister_code(KC_F13);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP23:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F14);
       } else {
        unregister_code(KC_F14);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_STRT0:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F15);
       } else {
        unregister_code(KC_F15);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP25:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F16);
       } else {
        unregister_code(KC_F16);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_IRPLY:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F17);
       } else {
        unregister_code(KC_F17);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP27:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F18);
       } else {
        unregister_code(KC_F18);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP28:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F19);
       } else {
        unregister_code(KC_F19);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP29:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F20);
       } else {
        unregister_code(KC_F20);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP30:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F21);
       } else {
        unregister_code(KC_F21);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP31:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F22);
       } else {
        unregister_code(KC_F22);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP32:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F23);
       } else {
        unregister_code(KC_F23);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TMP33:
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_F24);
       } else {
        unregister_code(KC_F24);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_TRMPT:
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_F23);
       } else {
        unregister_code(KC_F23);
        unregister_code(KC_LCTL);
			};
      return false;
		case M_MICCLS:  //mutemic
			if (record->event.pressed) {
        register_code(KC_LSFT);
        register_code(KC_F22);
       } else {
        unregister_code(KC_F22);
        unregister_code(KC_LSFT);
			};
      return false;
		case M_MICOPN:  //mutemic
			if (record->event.pressed) {
        register_code(KC_LCTL);
        register_code(KC_F22);
       } else {
        unregister_code(KC_F22);
        unregister_code(KC_LCTL);
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
/* _TP0 _TOP00      DEFAULT LAYER
*	,-------------------------------------------------------------------------.
*	|esCTL|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|tab_FN|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |FNWIN | tap dance FNWIN
*	|-------------------------------------------------------------------------+
*	| shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(!) |,(!?)|.(?) |QGWOX|SFTEN| combos (M+,)=! (,+.)=?; tap dance QGWOX
*	|-------------------------------------------------------------------------+
*	|     |     |     |alt -|/ _TP1 |   space   |* _TP1 |PLDIS|     |     |     | tap dance PLDIS
*	`-------------------------------------------------------------------------'
*/
	[_TOP00] = LAYOUT(
    KC_ESC ,M_PRVW0 ,M_CHAT0 ,M_GAME0 ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,M_BRB,
    LT(_TP1,KC_TAB),_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,M_TMP25 ,M_TMP23 ,M_TMP27 ,_______,
    M_TRMPT ,M_ALRT0 ,M_SMSNG0 ,M_SBRA0 ,M_VZIO0 ,M_VOIP0 ,_______ ,_______ ,_______ ,_______ ,_______ ,KC_ENT,
    M_MICOPN ,KC_ESC ,KC_TAB ,TD(TD_QGWOX) ,M_MIC0 ,M_PRVT1,M_RPLAY ,_______ ,_______ ,_______ ,_______),

/* _QTY _QWERTY        DEFAULT LAYER
*	,-------------------------------------------------------------------------.
*	| esc |  T  |  Q  |  W  |  E  |  R  |  J  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|tb_QYT|  G  |  A  |  S  |  D  |  F  |  H  |GCHAL|GCHTM|GCHAT|  ;  |FNWIN | macro GCHAL, GCHTM, GCHAT; tap dance FNWIN
*	|-------------------------------------------------------------------------+
*	| shift |  B  |  Z  |  X  |  C  |  V  |  N  |  M  |  ,  |  .  |QGWOX|SFTEN|
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |  /  |   space   |* _TP1 |GMDIS|     |     |     | tap dance GMDIS
*	`-------------------------------------------------------------------------'
*/
	[_QWERTY] = LAYOUT(
    TO(_TP0)  ,KC_Q ,KC_W ,KC_E ,KC_R ,KC_T ,KC_Y ,KC_U ,KC_I ,KC_O ,KC_P ,KC_BSPC,
    KC_TAB  ,KC_A ,KC_S ,KC_D ,KC_F ,KC_G ,KC_H ,KC_J ,KC_K ,KC_L ,KC_SCLN  ,KC_QUOT,
    KC_LSFT ,KC_Z ,KC_X ,KC_C ,KC_V ,KC_B ,KC_N ,KC_M ,KC_COMM  ,KC_DOT ,KC_DEL ,TO(_TP0),
    KC_LCTL ,KC_LGUI  ,KC_LALT  ,TD(TD_QGWOX) ,MO(_QYT) ,KC_SPC ,_______  ,KC_LEFT  ,KC_DOWN  ,KC_UP  ,KC_RGHT),

/* _TP1 _TOP01
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
	[_TOP01] = LAYOUT(
    _______ ,M_PRVW1 ,M_CHAT1 ,M_GAME1 ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,M_STRT0 ,M_CAP,
		LT(_TP1,KC_TAB),_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______,
		_______ ,M_ALRT1 ,M_SMSNG1 ,M_SBRA1 ,M_VZIO1 ,M_VOIP1 ,_______ ,_______ ,_______ ,_______ ,_______ ,_______,
    M_MICCLS ,_______ ,_______ ,TD(TD_QGWOX) ,M_MIC1 ,M_AFKTG  ,M_IRPLY ,_______ ,_______ ,_______ ,_______),

/* _FN _FUNCTION
*	,-------------------------------------------------------------------------.
*	|(tsk)|F1F11|F2F12| F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |PrtScr | (tsk) = task manager; tap dance F1F11, F2F12
*	|-------------------------------------------------------------------------+
*	| CAPS | n1  | n2  | n3  | n4  | n5  | n6  | n7  | n8  | n9  | n0  |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |home |pgup |pgdn | end |     |rsft |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  @  |  [   |     _     |  ]   |  =  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_FUNCTION] = LAYOUT(
    _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______,
		_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______,
		_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______,
    _______ ,_______ ,_______ ,TD(TD_QGWOX) ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______),

/*    mouse reserved
    |___| 5 | 6 | scrl|
    | 3 |pdn| 4 | f7  |
    | 1 |pup| 2 | f8  |   */

/* _QYT _QWERTY_TH
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
	[_QWERTY_TH] = LAYOUT(
    _______ ,KC_1 ,KC_2 ,KC_3 ,KC_4 ,KC_5 ,KC_6 ,KC_7 ,KC_8 ,KC_9 ,KC_0 ,_______,
    _______ ,TD(TD_F1F11),TD(TD_F2F12),KC_F3 ,KC_F4 ,KC_F5 ,KC_F6 ,KC_F7 ,KC_F8 ,KC_F9 ,KC_F10  ,_______,
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
    _______,_______,_______,TD(TD_QGWOX),_______,_______,_______,_______,_______,_______,_______),

// _CH _CHAT
	[_CHAT] = LAYOUT(
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,TD(TD_QGWOX),_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______),
};

//per layer underglow
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
      case _TP0:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 0);
//        rgblight_disable_noeeprom();
        break;
      case _QTY:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_MAGENTA);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 0);
        break;
      case _TP1:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 2);
        break;
      case _FN:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_CYAN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 1);
        break;
      case _QYT:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_MAGENTA);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT + 2);
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
