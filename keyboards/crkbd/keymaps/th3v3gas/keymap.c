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
  _EDIT,
  _THUMB,
  _FUNCTION,
  _GAME_FUNCTION,
  _CHAT,
  _BLNDS
};

#define _QW _QWERTY
#define _GM _GAME
#define _EDT _EDIT
#define _TH _THUMB
#define _FN _FUNCTION
#define _GFN _GAME_FUNCTION
#define _CH _CHAT

enum custom_keycodes {
  TD_F1F11, //double tap
  TD_F2F12,
  TD_CAPS,
  TD_QGWOX,
  TD_FNWIN,
  TD_CHENT,
  TD_RPRN,
  M_CHESC = SAFE_RANGE, //macro
  M_GCHAL,
  M_GCHTM,
  M_GCHAT,
  M_DSCRD,
  M_DSCEXT,
  M_EFIND,
  M_BLRUN,
  M_BLSLD,
  M_BLADS,
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
void td02_finished (qk_tap_dance_state_t *state, void *user_data); //(ref:TD_RPRN)
void td02_reset (qk_tap_dance_state_t *state, void *user_data);
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
//TD _GAME enter; enter; (ref:TD_CHENT)
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
//right bracket; _EDIT (ref:TD_RPRN)
void td02_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP: //plus
      register_code16(KC_RBRC);
      break;
    case SINGLE_HOLD:
      register_code16(KC_RBRC);
      break;
    case DOUBLE_SINGLE_TAP:
      NULL;
  }
}
void td02_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_RBRC);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_RBRC);
      break;
    case DOUBLE_SINGLE_TAP:
      layer_on(_EDT);
  }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_F1F11] = ACTION_TAP_DANCE_DOUBLE(KC_F1, KC_F11),
  [TD_F2F12] = ACTION_TAP_DANCE_DOUBLE(KC_F2, KC_F12),
  [TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(_______, KC_CAPS),
  [TD_QGWOX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td01_finished, td01_reset),
  [TD_FNWIN] = ACTION_TAP_DANCE_FN_ADVANCED_TIME (NULL, td03_finished, td03_reset, 160),
  [TD_CHENT] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td05_finished, td05_reset, 170),
  [TD_RPRN] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td02_finished, td02_reset, 150),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_CHESC: //close _CHAT, switch to _GAME
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
		case M_GCHAT: //_CHAT
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
			};
      return false;
		case M_DSCRD: //discord overlay
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CH);
        send_string(SS_TAP(X_F17));
			};
      return false;
		case M_DSCEXT: //exit discord overlay, switch to _GAME
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_GM);
        send_string(SS_TAP(X_F17));
			};
      return false;
		case M_EFIND: //_EDIT find
			if (record->event.pressed) {
        send_string(SS_LCTL("f"));
        layer_move(_QW);
			};
      return false;
		case M_BLRUN: //borderlands run forward
			if (record->event.pressed) {
        register_code(KC_W);
        send_string(SS_TAP(X_P5));
       } else {
        unregister_code(KC_W);
			};
      return false;
		case M_BLSLD: //borderlands slide
			if (record->event.pressed) {
        layer_off(_BLNDS);
        register_code(KC_B);
       } else {
        layer_on(_BLNDS);
        unregister_code(KC_B);
        send_string(SS_TAP(X_P5));
			};
      return false;
		case M_BLADS: //borderlands aim down sight
			if (record->event.pressed) {

        register_code(KC_SPC);
       } else {
        unregister_code(KC_SPC);
        send_string(SS_TAP(X_P5));
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
*	+-----------------------------------+  +-----------------------------------.
*	|esCTL|  Q  |  W  |  E  |  R  |  T  |  |  Y  |  U  |  I  |  O  |  P  |Bspce|
*	|-----------------------------------|  |-----------------------------------+
*	|tab_F|  A  |  S  |  D  |  F  |  G  |  |  H  |  J  |  K  |  L  |  ;  |FNWIN|  tap dance FNWIN
*	|-----------------------------------|  |-----------------------------------+
*	|shift|  Z  |  X  |  C  |  V  |  B  |  |  N  |M(!) |,(!?)|.(?) |QGWOX|SFTEN|  combos (M+,)=! (,+.)=?; tap dance QGWOX
*	+-----------------------------------|  |-----------------------------------+
*	                  |alt *|_TH /| del |  |space|_TH [|RPRN |                    tap dance RPRN
*	                  +-----------------+  +-----------------+
*/
	[_QWERTY] = LAYOUT(
     CTL_T(KC_ESC),KC_Q,KC_W,KC_E,KC_R,KC_T,  KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,
		LT(_FN,KC_TAB),KC_A,KC_S,KC_D,KC_F,KC_G,  KC_H,KC_J,KC_K,KC_L,KC_SCLN,TD(TD_FNWIN),
		       KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,  KC_N,KC_M,KC_COMM,KC_DOT,TD(TD_QGWOX),KC_SFTENT,
		 LALT_T(KC_PAST),LT(_TH,KC_SLSH),KC_DEL,  KC_SPC,LT(_TH,KC_LBRC),TD(TD_RPRN)),

/* _GM _GAME        DEFAULT LAYER
*	+-----------------------------------+  +-----------------------------------+
*	| esc |  T  |  Q  |  W  |  E  |  R  |  |  J  |  U  |  I  |  O  |BDLND|Bspce|
*	|-----------------------------------|  |-----------------------------------+
*	| _GF |  G  |  A  |  S  |  D  |  F  |  |  H  |GCHAL|GCHTM|GCHAT|  ;  |FNWIN|  macro GCHAL, GCHTM, GCHAT; tap dance FNWIN
*	|-----------------------------------|  |-----------------------------------+
*	|shift|  B  |  Z  |  X  |  C  |  V  |  |  N  |  M  |  ,  |  .  |QGWOX|SFTEN|
*	+-----------------------------------|  |-----------------------------------+
*	                  |  [  |  ]  | del |  |space|pause|DSCRD|                    macro DSCRD
*	                  +-----------------+  +-----------------+
*/
	[_GAME] = LAYOUT(
      KC_ESC,KC_T,KC_Q,KC_W,KC_E,KC_R,  KC_J,KC_U,KC_I,KC_O,TG(_BLNDS),KC_BSPC,
    MO(_GFN),KC_G,KC_A,KC_S,KC_D,KC_F,  KC_H,M_GCHAL,M_GCHTM,M_GCHAT,KC_SCLN,TD(TD_FNWIN),
     KC_LSFT,KC_B,KC_Z,KC_X,KC_C,KC_V,  KC_N,KC_M,KC_COMM,KC_DOT,TD(TD_QGWOX),KC_SFTENT,
               KC_LBRC,KC_RBRC,KC_DEL,  KC_SPC,LT(_TH,KC_PAUS),M_DSCRD),

/* _EDT _EDIT
*	+-----------------------------------+  +-----------------------------------+
*	|     |  [  |  ]  |  (  |  )  |  /  |  |undo |     |     |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |slall|save |     |find | del |  |left |down | up  |right|     |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |undo | cut |     |bspce|enter|  |     |     |     |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |copy |paste|space|  |     |     |     |
*	                  +-----------------+  +-----------------+
*/
	[_EDIT] = LAYOUT(
    _______,KC_LBRC,KC_RBRC,KC_LPRN,KC_RPRN,KC_SLSH,  C(KC_Y),_______,_______,_______,_______,TO(_QW),
     KC_TRNS,C(KC_A),C(KC_S),_______,M_EFIND,KC_DEL,  KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,_______,KC_TRNS,
  	 KC_TRNS,C(KC_Z),C(KC_X),_______,KC_BSPC,KC_ENT,  KC_HOME,KC_PGDN,KC_PGUP,KC_END,KC_TRNS,_______,
                             C(KC_C),C(KC_V),KC_SPC,  _______,KC_TRNS,TO(_QW)),

/* _TH _THUMB
*	+-----------------------------------+  +-----------------------------------+
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  |  6  |  7  |  8  |  9  |  0  |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |     |     |     |     |  |left |down | up  |right|     |  \| |
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |     |copy |paste|     |  |     |     |     |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  |  +  |  -  |space|  | del |  (  |  )  |
*	                  +-----------------+  +-----------------+
*/
	[_THUMB] = LAYOUT(
                    KC_GRV,KC_1,KC_2,KC_3,KC_4,KC_5,  KC_6,KC_7,KC_8,KC_9,KC_0,_______,
		_______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,  KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,_______,KC_TRNS,
		_______,XXXXXXX,XXXXXXX,C(KC_C),C(KC_V),XXXXXXX,  KC_HOME,KC_PGDN,KC_PGUP,KC_END,KC_TRNS,_______,
		                         KC_PPLS,KC_MINS,KC_SPC,  KC_UNDS,KC_LPRN,KC_RPRN),

/* _FN _FUNCTION
*	+-----------------------------------+  +-----------------------------------+
*	|(tsk)|F1F11|F2F12| F3  | F4  | F5  |  | F6  | F7  | F8  | F9  | F10 |PrtSc|  (tsk) = task manager; tap dance F1F11, F2F12
*	|-----------------------------------|  |-----------------------------------+
*	|CAPS | n1  | n2  | n3  | n4  | n5  |  | n6  | n7  | n8  | n9  | n0  |     |  tap dance TD_CAPS
*	|-----------------------------------|  |-----------------------------------+
*	|     |     |     |     |     |     |  |     |     |     |     |QGWOX|rsft |
*	+-----------------------------------|  |-----------------------------------+
*	                  |  @  |  =  |  _  |  |  _  |     |     |
*	                  +-----------------+  +-----------------+
*/
	[_FUNCTION] = LAYOUT(
    C(S(KC_ESC)),TD(TD_F1F11),TD(TD_F2F12),KC_F3,KC_F4,KC_F5,  KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_PSCR,
		               TD(TD_CAPS),KC_P1,KC_P2,KC_P3,KC_P4,KC_P5,  KC_P6,KC_P7,KC_P8,KC_P9,KC_P0,KC_TRNS,
		         _______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,  XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_TRNS,KC_RSFT,
		                                    KC_AT,KC_EQL,KC_UNDS,  KC_UNDS,_______,_______),

/* _GFN _GAME_FUNCTION
*	+-----------------------------------+  +-----------------------------------+
*	| del | f1  |  7  |  8  |  9  | f4  |  |     |     |     |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	|     | f2  |  4  |  5  |  6  | f5  |  | F11 |     |     |     |     |     |
*	|-----------------------------------|  |-----------------------------------+
*	| tab | f3  |  1  |  2  |  3  | f6  |  | F12 |     |     |     |QGWOX|     |
*	+-----------------------------------|  |-----------------------------------+
*	                  | n/  |   0 |  _  |  |space|     |     |
*	                  +-----------------+  +-----------------+
*/
	[_GAME_FUNCTION] = LAYOUT(
     KC_DEL,KC_F1,KC_7,KC_8,KC_9,KC_F4,  _______,_______,_______,_______,_______,_______,
    _______,KC_F2,KC_4,KC_5,KC_6,KC_F5,  KC_F11,_______,_______,_______,_______,KC_TRNS,
     KC_TAB,KC_F3,KC_1,KC_2,KC_3,KC_F6,  KC_F12,_______,_______,_______,KC_TRNS,_______,
                  KC_PSLS,KC_0,KC_PAST,  KC_SPC,_______,_______),

	[_CHAT] = LAYOUT( // _CH _CHAT
    M_CHESC,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_TRNS,TD(TD_CHENT),
		_______,_______,_______,_______,_______,M_DSCEXT),
	[_BLNDS] = LAYOUT( //borderlands
    _______,_______,_______,M_BLRUN,_______,_______,_______,_______,_______,_______,TG(_BLNDS),_______,
		_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
		_______,M_BLSLD,_______,_______,_______,_______,_______,_______,_______,_______,KC_TRNS,_______,
		_______,_______,M_BLADS,_______,_______,M_DSCEXT),
};


#ifdef OLED_DRIVER_ENABLE
char keylog_str[5] = {};
uint16_t oled_timer;
uint16_t log_timer = 0;
void update_log(void) {
    if (timer_elapsed(log_timer) > 750) {
        //add_keylog(0);
    }
}
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        //add_keylog(keycode);
        oled_timer = timer_read();
    }
    return true;
}
void render_default_layer_state(void) {
    oled_write("Layer:", false);
    switch (get_highest_layer(layer_state)) {
        case _QW:
            oled_write_P(PSTR("_QWERTY\n"), false);
            break;
        case _GM:
            oled_write_P(PSTR("_GAME\n"), false);
            break;
        case _BLNDS:
            oled_write_P(PSTR("Borderlands\n"), false);
            break;
        case _EDT:
            oled_write_P(PSTR("_EDIT\n"), false);
            break;
        case _TH:
            oled_write_P(PSTR("_THUMB\n"), false);
            break;
        case _FN:
            oled_write_P(PSTR("_FUNCTION\n"), false);
            break;
        case _GFN:
            oled_write_P(PSTR("_GAME_FUNCTION\n"), false);
            break;
        case _CH:
            oled_write_P(PSTR("_CHAT\n"), false);
            break;
    }
}
void render_keylock_status(uint8_t led_usb_state) {
    oled_write_P(PSTR("Lock:"), false);
    oled_write_P(PSTR(" NUM "), led_usb_state & (1 << USB_LED_NUM_LOCK));
    oled_write_P(PSTR(" CAPS "), led_usb_state & (1 << USB_LED_CAPS_LOCK));
    oled_write_P(PSTR("     "), false);
}
void render_mod_status(uint8_t modifiers) {
    oled_write_P(PSTR("Mods:"), false);
    oled_write_P(PSTR(" S "), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR(" C "), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR(" A "), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR(" W "), (modifiers & MOD_MASK_GUI));
    oled_write_P(PSTR("    "), false);
}
void render_status_main(void) {
    render_default_layer_state();
    render_keylock_status(host_keyboard_leds());
    render_mod_status(get_mods() | get_oneshot_mods());
}
void render_status_secondary(void) {
    oled_write("Bite me.", false);
}
void oled_task_user(void) {
    if (timer_elapsed(oled_timer) > 60000) {
        oled_off();
        return;
    }
    if (is_master) {
        render_status_main();
    } else {
        render_status_secondary();
    }
}
#endif
