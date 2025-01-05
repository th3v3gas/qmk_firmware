#include QMK_KEYBOARD_H

extern uint8_t is_master;

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
  _GF76,
  _EDIT,
  _BLWL,
  _NUMB,
  _FUNCTION,
  _FNGM,
  _FN76,
  _CHAT,
};

#define _QW _QWERTY
#define _GM _GAME
#define _NM _NUMB
#define _FN _FUNCTION
#define _CH _CHAT

enum custom_keycodes {
  TD_F11TD, //tap dance
  TD_F12TD,
  TD_LOCK,
  TD_QGWOX,
  TD_FNWIN,
  M_CHESC = SAFE_RANGE, //macro
  M_GCHAT,
  M_EFIND,
  M_BLRUN,
  M_BLRT,
  M_BLLF,
  M_BLSLD,
  M_BLADS,
  M_76NAD,
  M_EDIT,
  M_GF76,
  M_VOL,
  M_MIC,
  M_CHENT,
  M_PPT,
  M_BLWL,
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
//int cur_dance (tap_dance_state_t *state);
td_state_t cur_dance(tap_dance_state_t *state);
void td01_finished (tap_dance_state_t *state, void *user_data); //(ref:TD_QGWOX)
void td01_reset (tap_dance_state_t *state, void *user_data);
void td03_finished (tap_dance_state_t *state, void *user_data); //(ref:TD_FNWIN)
void td03_reset (tap_dance_state_t *state, void *user_data);
//int cur_dance (tap_dance_state_t *state) {
td_state_t cur_dance(tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) return SINGLE_TAP;
      else return SINGLE_HOLD;
  }
  if (state->count == 2) return DOUBLE_SINGLE_TAP;
  else return 3;
}
//TD _QWERTY; _GAME; wox (ref:TD_QGWOX)
void td01_finished (tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //_QWERTY
      layer_clear();
      break;
    case SINGLE_HOLD: //_GAME
      layer_move(_GM);
      break;
    case DOUBLE_SINGLE_TAP: //wox
      layer_clear();
      layer_move(_QW);
      register_code16(KC_RCTL);
      register_code16(KC_F24);
      unregister_code16(KC_F24);
      unregister_code16(KC_RCTL);
  }
}
void td01_reset (tap_dance_state_t *state, void *user_data) {
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
void td03_finished (tap_dance_state_t *state, void *user_date) {
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
void td03_reset (tap_dance_state_t *state, void *user_data) {
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
tap_dance_action_t tap_dance_actions[] = {
  [TD_F11TD] = ACTION_TAP_DANCE_DOUBLE(KC_F9, KC_F11),
  [TD_F12TD] = ACTION_TAP_DANCE_DOUBLE(KC_F10, KC_F12),
  [TD_LOCK] = ACTION_TAP_DANCE_DOUBLE(KC_CAPS, KC_NUM),
  [TD_QGWOX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
  [TD_FNWIN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td03_finished, td03_reset),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_CHESC: //close _CHAT, switch to _GAME
			if (record->event.pressed) {
        layer_move(_GM);
        send_string(SS_TAP(X_ESC)); //use this format for tapping a key in a macro
			};
      return false;
		case M_GCHAT: //KC_enter, switch to qwerty for game chat
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
        send_string(SS_TAP(X_ENT));
			};
      return false;
		case M_EFIND: //ctlfind, switch to qwerty
			if (record->event.pressed) {
        send_string(SS_LCTL("f"));
        layer_move(_QW);
			};
      return false;
		case M_BLRUN: //border/wonderlands run forward
			if (record->event.pressed) {
        register_code(KC_W);
        send_string(SS_TAP(X_P5));
       } else {
        unregister_code(KC_W);
			};
      return false;
		case M_BLRT: //border/wonderlands run right
			if (record->event.pressed) {
        register_code(KC_D);
        send_string(SS_TAP(X_P5));
       } else {
        unregister_code(KC_D);
        send_string(SS_TAP(X_P5));
			};
      return false;
		case M_BLLF: //border/wonderlands run left
			if (record->event.pressed) {
        register_code(KC_A);
        send_string(SS_TAP(X_P5));
      } else {
        unregister_code(KC_A);
        send_string(SS_TAP(X_P5));
			};
      return false;
		case M_BLSLD: //border/wonderlands slide
			if (record->event.pressed) {
        layer_move(_GAME);
        register_code(KC_B);
      } else {
        layer_move(_BLWL);
        unregister_code(KC_B);
        send_string(SS_TAP(X_P5));
			};
      return false;
		case M_BLADS: //border/wonderlands aim down sight
			if (record->event.pressed) {
        register_code(KC_SPC);
      } else {
        unregister_code(KC_SPC);
        send_string(SS_TAP(X_P5));
			};
      return false;
		case M_76NAD: //fo76 grenade
			if (record->event.pressed) {
        register_code(KC_C);
        wait_ms(600);
      } else {
        unregister_code(KC_C);
			};
      return false;
		case M_EDIT: //swap to _EDIT
			if (record->event.pressed) {
        layer_invert(_EDIT);
			};
      return false;
		case M_GF76: //swap to Fallout 76
			if (record->event.pressed) {
        layer_clear();
        layer_move(_GF76);
			};
      return false;
		case M_BLWL: //swap to border/wonderlands
			if (record->event.pressed) {
        layer_clear();
        layer_move(_BLWL);
			};
      return false;
		case M_VOL: //ear trumpet hotkey
			if (record->event.pressed) {
        register_code(KC_RCTL);
        send_string(SS_TAP(X_F23));
        unregister_code(KC_RCTL);
			};
      return false;
		case M_MIC: //mute mic hotkey
			if (record->event.pressed) {
        register_code(KC_RSFT);
        send_string(SS_TAP(X_F23));
        unregister_code(KC_RSFT);
			};
      return false;
		case M_CHENT: //enter, switch to _GAME from _CHAT
			if (record->event.pressed) {
        layer_move(_GM);
        send_string(SS_TAP(X_ENT));
			};
      return false;
		case M_PPT:  //obs ptt  (ctl+f17-18) and KC_T for push to talk hotkeys
			if (record->event.pressed) {
        register_code(KC_LCTL);
        tap_code(KC_F17); //hotkey (ctl+f17) in obs, enable voice
        unregister_code(KC_LCTL);
        register_code(KC_T);
       } else {
        unregister_code(KC_T);
        register_code(KC_LCTL);
        tap_code(KC_F18); //hotkey (ctl+f18) in obs, disable voice
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
  co_qm, //question mark
  co_ep, //exclamation point
  co_es, //escape?!
};
const uint16_t PROGMEM qm_combo[] = {KC_COMM, KC_DOT, COMBO_END};//?
const uint16_t PROGMEM ex_combo[] = {KC_M, KC_COMM, COMBO_END};//!
const uint16_t PROGMEM es_combo[] = {KC_Z, KC_X, COMBO_END};//esc
combo_t key_combos[COMBO_COUNT] = {
  [co_qm] = COMBO(qm_combo, KC_QUES),//?
  [co_ep] = COMBO(ex_combo, KC_EXLM),//!
  [co_es] = COMBO(es_combo, KC_ESC),//esc
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*	_QW _QWERTY  BASE LAYER  it's qwerty!
*	+-----------------------------------+  +-----------------------------------.
*	| esc |  q  |  w  |  e  |  r  |  t  |  |  y  |  u  |  i  |  o  |  p  |bspce|  tap dance FNWIN
*	|-----------------------------------|  |-----------------------------------+
*	|tabFN|  a  |  s  |  d  |  f  |  g  |  |  h  |  j  |  k  |  l  |  ;  |FNWIN|  combos (z+x)=escape
*	|-----------------------------------|  |-----------------------------------+  (M+,)=! (,+.)=?
*	|shift|z(es)|x(es)|  c  |  v  |  b  |  |  n  |m(!) |,(!?)|.(?) |QGWOX|sfent|
*	+-----------------------------------|  |-----------------------------------+  tap dance QGWOX
*	                  |ctl *|_NM /| del |  |space|_NM [|alt ]|
*	                  +-----------------+  +-----------------+
*/
	[_QWERTY] = LAYOUT(
          KC_ESC,KC_Q,KC_W,KC_E,KC_R,KC_T,  KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
  LT(_FN,KC_TAB),KC_A,KC_S,KC_D,KC_F,KC_G,  KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
         KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,  KC_N,KC_M,KC_COMM,KC_DOT,TD(TD_QGWOX),SC_SENT,
    CTL_T(KC_PAST),LT(_NM,KC_SLSH),KC_DEL,  KC_SPC,LT(_NM,KC_LBRC),LALT_T(KC_RBRC)),

/*	_NM _NUMB                number row, nav cluster
*	+-----------------------------------+  +-----------------------------------+
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  |  6  |  7  |  8  |  9  |  0  |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |home |pgdn |pgup | end |  |left |down | up  |right|     |  \| |
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |     |     |  -  |  =  |  |     |     |  ,  |  .  |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |  +  |  -  | del |  |space|  (  |  )  |
*	                  +-----------------+  +-----------------+
*/
	[_NUMB] = LAYOUT(
                 KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,  KC_6,KC_7,KC_8,KC_9,KC_0,_______,
  _______,XXXXXXX,KC_HOME,KC_PGDN,KC_PGUP,KC_END,  KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,XXXXXXX,KC_BSLS,
  _______,XXXXXXX,XXXXXXX,XXXXXXX,KC_MINS,KC_EQL,  XXXXXXX,XXXXXXX,KC_COMM,KC_DOT,_______,_______,
                          KC_PPLS,KC_MINS,KC_DEL,  KC_SPC,KC_LPRN,KC_RPRN),

/*	_FN _FUNCTION            function row, number pad
*	+-----------------------------------+  +-----------------------------------+
*	|(tsk)| f3  | f2  | F3  | F4  | F5  |  | F6  | F7  | F8  |F11TD|F12TD|prtsc|  (tsk) = ctl+shift+esc
*	|-----------------------------------|  |-----------------------------------+  tap dance F11TD, F12TD
*	|     | n1  | n2  | n3  | n4  | n5  |  | n6  | n7  | n8  | n9  | n0  |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |  n/ |  n* |  n- |  n+ |  n. |  |LOCK |     | VOL | MIC |QGWOX|sfent|  macro VOL, MIC,
*	+-----------------------------------|  |-----------------------------------+  tap dance LOCK
*	                  |  @  |  =  |space|  |  _  |     |     |
*	                  +-----------------+  +-----------------+
*/
	[_FUNCTION] = LAYOUT(
       C(S(KC_ESC)),KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,  KC_F6,KC_F7,KC_F8,TD(TD_F11TD),TD(TD_F12TD),KC_PSCR,
            XXXXXXX,KC_P1,KC_P2,KC_P3,KC_P4,KC_P5,  KC_P6,KC_P7,KC_P8,KC_P9,KC_P0,M_EDIT,
  _______,KC_PSLS,KC_PAST,KC_PMNS,KC_PPLS,KC_PDOT,  TD(TD_LOCK),XXXXXXX,M_VOL,M_MIC,_______,_______,
                              KC_AT,KC_EQL,KC_SPC,  KC_UNDS,KC_RALT,KC_RCTL),

/*	_EDIT                    for editing (stealing code)
*	+-----------------------------------+  +-----------------------------------+
*	|     |  [  |  ]  |  (  |  )  |bspce|  |redo |     |     |     |     | _QW |
*	|-----------------------------------|  |-----------------------------------+
*	|     |slall|save |     |find |     |  |  /  |  \  |  *  |  ;  |  '  |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |undo | cut |copy |paste|enter|  |     |     |     |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |     |space| del |  |     |     |     |
*	                  +-----------------+  +-----------------+
*/
	[_EDIT] = LAYOUT(
  _______,KC_LBRC,KC_RBRC,KC_LPRN,KC_RPRN,KC_BSPC,  C(KC_Y),XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,TO(_QW),
  _______,C(KC_A),C(KC_S),XXXXXXX,M_EFIND,XXXXXXX,  KC_SLSH,KC_BSLS,KC_PAST,KC_SCLN,KC_QUOT,XXXXXXX,
   _______,C(KC_Z),C(KC_X),C(KC_C),C(KC_V),KC_ENT,  XXXXXXX,XXXXXXX,_______,_______,_______,_______,
                            XXXXXXX,KC_SPC,KC_DEL,  XXXXXXX,_______,_______),

/*	_GM _GAME    BASE LAYER  gaming layout for humans
*	+-----------------------------------+  +-----------------------------------+
*	|  .  | PTT |  q  |  w  |  e  |  r  |  |  y  |  u  |  i  |  o  |  p  |bspce|  macro PTT
*	|-----------------------------------|  |-----------------------------------+
*	|_FNGM|  g  |  a  |  s  |  d  |  f  |  |  h  |  J  |  k  |  l  |  ;  |FNWIN|
*	|-----------------------------------|  |-----------------------------------+
*	|shift|  b  |  z  |  x  |  c  |  v  |  |  n  |  m  | VOL | MIC |QGWOX|GCHAT|  macro GCHAT
*	+-----------------------------------|  |-----------------------------------+
*	                  | ctr | alt |space|  |pause|_NM [|alt ]|
*	                  +-----------------+  +-----------------+
*/
	[_GAME] = LAYOUT(
    KC_DOT,M_PPT,KC_Q,KC_W,KC_E,KC_R,  KC_J,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
  MO(_FNGM),KC_G,KC_A,KC_S,KC_D,KC_F,  KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
    KC_LSFT,KC_B,KC_Z,KC_X,KC_C,KC_V,  KC_N,KC_M,M_VOL,M_MIC,TD(TD_QGWOX),M_GCHAT,
              KC_LCTL,KC_LALT,KC_SPC,  KC_PAUS,LT(_NM,KC_LBRC),LALT_T(KC_RBRC)),

/*	_FNGM                    gaming number and function
*	+-----------------------------------+  +-----------------------------------+
*	| tab |  1  |  2  |  3  |  4  |  5  |  | f4  | f5  | f6  |GF76 |BLWL |     |  macro GF76, BLWL
*	|-----------------------------------|  |-----------------------------------+
*	|     |  6  |  7  |  8  |  9  |  0  |  | f7  | f8  | f9  |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	|home | f1  | f2  | f3  |  -  |  =  |  | f10 | f11 | f12 |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |  ;  |  '  |  ,  |  | end | alt | esc |
*	                  +-----------------+  +-----------------+
*/
	[_FNGM] = LAYOUT(
           KC_TAB,KC_1,KC_2,KC_3,KC_4,KC_5,  KC_F4,KC_F5,KC_F6,M_GF76,M_BLWL,XXXXXXX,
          _______,KC_6,KC_7,KC_8,KC_9,KC_0,  KC_F7,KC_F8,KC_F9,XXXXXXX,XXXXXXX,XXXXXXX,
  KC_HOME,KC_F1,KC_F2,KC_F3,KC_MINS,KC_EQL,  KC_F10,KC_F11,KC_F12,XXXXXXX,XXXXXXX,XXXXXXX,
                   KC_SCLN,KC_QUOT,KC_COMM,  KC_END,KC_TAB,KC_ESC),

/*	_GF76        BASE LAYER  fallout 76
*	+-----------------------------------+  +-----------------------------------+
*	| esc | PPT |  x  |  w  |  e  |  r  |  |  y  |  u  |  i  |  o  |  p  |bspce|
*	|-----------------------------------|  |-----------------------------------+
*	|_FN76|  q  |  a  |  s  |  d  |76NAD|  |  h  |  j  |  k  |  l  |  ;  |FNWIN|  macro 76NAD
*	|-----------------------------------|  |-----------------------------------+
*	|shift|  z  |  8  | tab | alt |  c  |  |  f  |  v  | VOL | MIC |QGWOX|sften|
*	+-----------------------------------|  |-----------------------------------+
*	                  |  1  |  =  |space|  |pause|_NM [|alt ]|
*	                  +-----------------+  +-----------------+
*/
	[_GF76] = LAYOUT(
       KC_ESC,M_PPT,KC_X,KC_W,KC_E,KC_R,  KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
  MO(_FN76),KC_Q,KC_A,KC_S,KC_D,M_76NAD,  KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
  KC_LSFT,KC_Z,KC_8,KC_TAB,KC_LALT,KC_C,  KC_F,KC_V,M_VOL,M_MIC,TD(TD_QGWOX),SC_SENT,
                     KC_1,KC_EQL,KC_SPC,  KC_PAUS,LT(_NM,KC_LBRC),LALT_T(KC_RBRC)),

/*	_FN76                    fallout 76 numbers and function
*	+-----------------------------------+  +-----------------------------------+
*	| f3  |  1  |  2  |  3  |  4  |  5  |  | f4  | f5  | f6  |GF76 |BLWL |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |  6  |  7  |  8  |  0  |  9  |  | f7  | f8  | f9  |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	| n0  |  m  |  n  |  ,  |  -  | f6  |  | f10 | f11 | f12 |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |  v  |  z  |  +  |  | end | alt | esc |
*	                  +-----------------+  +-----------------+
*/
	[_FN76] = LAYOUT(
         KC_F3,KC_1,KC_2,KC_3,KC_4,KC_5,  KC_F4,KC_F5,KC_F6,M_GF76,M_BLWL,XXXXXXX,
       _______,KC_6,KC_7,KC_8,KC_0,KC_9,  KC_F7,KC_F8,KC_F9,XXXXXXX,XXXXXXX,XXXXXXX,
  KC_P0,KC_M,KC_N,KC_COMM,KC_MINS,KC_F6,  KC_F10,KC_F11,KC_F12,XXXXXXX,XXXXXXX,XXXXXXX,
                      KC_V,KC_Z,KC_PSLS,  KC_END,KC_TAB,KC_ESC),

/*  _BLWL        BASE LAYER  border/wonderlands
*	+-----------------------------------+  +-----------------------------------+
*	|  .  | PPT |  q  |BLRUN|  e  |  r  |  |  y  |  u  |  i  |  o  |  p  |bspce|  various macros
*	|-----------------------------------|  |-----------------------------------+  ref:M_BL
*	|_FNGM|  g  |BLLF |  s  | BLRT|  f  |  |  h  |  j  |  k  |  l  |  ;  |FNWIN|
*	|-----------------------------------|  |-----------------------------------+
*	|shift|BLSLD|  z  |  x  |  c  |  v  |  |  n  |  m  | VOL | MIC |QGWOX| _QW |
*	+-----------------------------------|  |-----------------------------------+
*	                  | ctr | alt |BLADS|  |pause|_NM [|alt ]|
*	                  +-----------------+  +-----------------+
*/
  [_BLWL] = LAYOUT(
     KC_DOT,M_PPT,KC_Q,M_BLRUN,KC_E,KC_R,  KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
  MO(_FNGM),KC_G,M_BLLF,KC_S,M_BLRT,KC_F,  KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
     KC_LSFT,M_BLSLD,KC_Z,KC_X,KC_C,KC_V,  KC_N,KC_M,M_VOL,M_MIC,TD(TD_QGWOX),TO(_QW),
                 KC_LCTL,KC_LALT,M_BLADS,  KC_PAUS,LT(_NM,KC_LBRC),LALT_T(KC_RBRC)),

/*  _CH _CHAT                qwerty overlay for game chat GCHAT
*	+-----------------------------------+  +-----------------------------------+
*	|     |     |     |     |     |     |  |     |     |     |     |     |     |  blank layout
*	|-----------------------------------|  |-----------------------------------+  for new layers
*	|     |     |     |     |     |     |  |     |     |     |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |     |     |     |     |  |     |     |     |     |     |     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |     |     |     |  |     |     |     |
*	                  +-----------------+  +-----------------+
*/
	[_CHAT] = LAYOUT(
  M_CHESC,_______,_______,_______,_______,_______,  _______,_______,_______,_______,_______,_______,
  _______,_______,_______,_______,_______,_______,  _______,_______,_______,_______,_______,_______,
  _______,_______,_______,_______,_______,_______,  _______,_______,_______,_______,_______,M_CHENT,
                          _______,_______,_______,  _______,_______,_______),
};
