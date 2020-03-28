#include QMK_KEYBOARD_H

//unicode WinCompose
//<https://github.com/samhocevar/wincompose>  default compose key is [KC_RALT]
//rules.mk (UNICODE_ENABLE = yes)
//config.h (#define UNICODE_KEY_WINC KC_RALT) <-- edit config.h to match.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};
enum layers_keymap {
  _QWERTY = 0,
  _GAME,
  _RTHUMB,
  _LTHUMB,
  _FUNCTION,
  _CHAT
};

#define _QW _QWERTY
#define _GM _GAME
#define _RT _RTHUMB
#define _LT _LTHUMB
#define _FUN _FUNCTION
#define _CHT _CHAT

enum custom_keycodes {
  TD_CAPS,
  TD_BASE,
  TD_RTGAME,
  TD_LTASTX,
  M_GUIL0 = SAFE_RANGE, //macro
  M_CHAT,
  M_CHENT,
  M_CHESC,
};

//tap dance
//rules.mk (TAP_DANCE_ENABLE = yes)
//config.h (#define TAPPING_TERM 170) double tap window 150-200
typedef enum { //tap dance game layer toggle
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;
static td_state_t td_state;
int cur_dance (qk_tap_dance_state_t *state);
void tdrt_finished (qk_tap_dance_state_t *state, void *user_data);
void tdrt_reset (qk_tap_dance_state_t *state, void *user_data);
void tdlt_reset_finished (qk_tap_dance_state_t *state, void *user_data);
void tdlt_reset (qk_tap_dance_state_t *state, void *user_data);
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
void tdrt_finished (qk_tap_dance_state_t *state, void *user_date) { //equal key down
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_EQL); //S-=
      break;
    case SINGLE_HOLD:
      layer_on(_RT); //H-numbers
      break;
    case DOUBLE_SINGLE_TAP:
      layer_move(_GM); //D-game layer
  }
}
void tdrt_reset (qk_tap_dance_state_t *state, void *user_data) { //equal key up
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_EQL); //S-=
      break;
    case SINGLE_HOLD:
      layer_off(_RT); //H-numbers
      break;
    case DOUBLE_SINGLE_TAP:
      layer_move(_GM); //D-game layer
  }
}
void tdlt_finished (qk_tap_dance_state_t *state, void *user_date) { //forward slash key down
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_PAST); //S-*
      break;
    case SINGLE_HOLD:
      layer_on(_LT); //H-numbers
      break;
    case DOUBLE_SINGLE_TAP:
      register_code16(KC_PAST); //D-*
  }
}
void tdlt_reset (qk_tap_dance_state_t *state, void *user_data) { //astrix key down
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_PAST); //S-*
      break;
    case SINGLE_HOLD:
      layer_off(_LT); //H-numbers
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_PAST); //D-*
  }
}

void TDBASE (qk_tap_dance_state_t *state, void *user_data) { //astrix key up
  switch(state->count){
    case 1:
      layer_move(0); //S-base layer
      break;
    case 2:
      send_string("=)"); //D-emote
    }
}
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_TRNS, KC_CAPS),
  [TD_BASE] = ACTION_TAP_DANCE_FN(TDBASE),
  [TD_RTGAME] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tdrt_finished, tdrt_reset),
  [TD_LTASTX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tdlt_finished, tdlt_reset),
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_GUIL0:
			if (record->event.pressed) {
        layer_clear();
        send_string(SS_TAP(X_LGUI)); //game win key
			};
      return false;
    case M_CHAT:
			if (record->event.pressed) {
        layer_move(_QW);
        layer_on(_CHT);
        send_string(SS_TAP(X_Y)); //game chat
			};
      return false;
    case M_CHENT:
			if (record->event.pressed) {
        layer_move(_GM);
        layer_off(_CHT);
        send_string(SS_TAP(X_ENT)); //game chat enter
			};
      return false;
    case M_CHESC:
			if (record->event.pressed) {
        layer_move(_GM);
        layer_off(_CHT);
        send_string(SS_TAP(X_ESC)); //game chat escape
			};
      return false;
  }
  return true;
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
/*Base 0
*	,-------------------------------------------------------------------------.
*	|escL4|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace |
*	|-------------------------------------------------------------------------+
*	|  Tab |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '   |
*	|-------------------------------------------------------------------------+
*	| Shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(?) |,(?!)|.(!) |BASE | Ent |
*	|-------------------------------------------------------------------------+
*	|     |     |     |/ ALT |l3  *|   Space   |l2=GM |  @  |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_QWERTY] = LAYOUT(LT(4,KC_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LCTL_T(KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_BASE), KC_SFTENT,
		KC_TRNS, KC_TRNS, KC_TRNS, LALT_T(KC_PSLS), TD(TD_LTASTX), KC_SPC, TD(TD_RTGAME), KC_AT, KC_TRNS, KC_TRNS, KC_TRNS),

/*Game 1
*	,-------------------------------------------------------------------------.
*	|     |  -  |  Q  |  W  |  E  |  R  |  +  |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      |  G  |  A  |  S  |  D  |  F  |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |  B  |  Z  |  X  |  C  |  V  |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |           |      |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_GAME] = LAYOUT(KC_TRNS, KC_PMNS, KC_Q, KC_W, KC_E, KC_R, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_G, KC_A, KC_S, KC_D, KC_F, KC_TRNS, M_CHAT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_B, KC_Z, KC_X, KC_C, KC_V, KC_TRNS, KC_M, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
*	Right thumb 2
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  | n7  | n8  | n9  | n0  |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     | n4  | n5  | n6  |     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     | n1  | n2  | n3  |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |  +   |     _     |      |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_RTHUMB] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_P7, KC_P8, KC_P9, KC_P0, KC_TRNS,
		KC_TRNS, KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS, KC_P4, KC_P5, KC_P6, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P1, KC_P2, KC_P3, KC_TRNS, M_GUIL0,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_PMNS, KC_PPLS, KC_UNDS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
*	Left thumb 3
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |left |down | up  |right|     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |  -  |  +   |     _     |Pause |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_LTHUMB] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
		KC_TRNS, KC_6, KC_7, KC_8, KC_9, KC_0, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_BSLS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_PMNS, KC_PPLS, KC_UNDS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
*	Function 4
*	,-------------------------------------------------------------------------.
*	|     | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |PrtScr |
*	|-------------------------------------------------------------------------+
*	| (CL) | F11 | F12 |     |     |     |     |     |     |     |     |      | tap dance CapLock
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     | RGB |
*	|-------------------------------------------------------------------------+
*	|     |     |     | WIN |      |           |      |     |     |     |     |
*	`-------------------------------------------------------------------------'
*/
	[_FUNCTION] = LAYOUT(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_PSCR,
		TD(TD_CAPS), KC_F11, KC_F12, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_RMOD, RGB_MOD, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_VAD, RGB_VAI, RGB_TOG,
		KC_TRNS, KC_TRNS, KC_TRNS, M_GUIL0, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
*	Game chat 5
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
