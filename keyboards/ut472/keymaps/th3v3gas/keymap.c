#include QMK_KEYBOARD_H

//unicode WinCompose
//<https://github.com/samhocevar/wincompose>  default compose key is [KC_RALT]
//rules.mk (UNICODE_ENABLE = yes)
//config.h (#define UNICODE_KEY_WINC KC_RALT) <-- edit config.h to match.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_WINC);
};

enum custom_keycodes {
  TD_LBRC,
  TD_RBRC,
  TD_CAPS,
  TD_NP1,
  TD_NP2,
  TD_NP3,
  TD_NP4,
  TD_EMOTE,
  TD_QUOTL2,
  M_GUIL0 = SAFE_RANGE, //macro
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
void tdgame_finished (qk_tap_dance_state_t *state, void *user_data);
void tdgame_reset (qk_tap_dance_state_t *state, void *user_data);
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
      else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; }
}
void tdgame_finished (qk_tap_dance_state_t *state, void *user_date) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_QUOT); //quote
      break;
    case SINGLE_HOLD: //numbers
      layer_on(2);
      break;
    case DOUBLE_SINGLE_TAP: //Game layer
      layer_on(1);
  }
}
void tdgame_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_QUOT);
      break;
    case SINGLE_HOLD:
      layer_off(2);
      break;
    case DOUBLE_SINGLE_TAP:
      layer_on(1);
  }
}
void TDEMOTE (qk_tap_dance_state_t *state, void *user_data) { //tap dance emote
  switch(state->count){
    case 1:
      send_unicode_string("¯\\_(ツ)_/¯");
      break;
    case 2:
      send_string("=)");
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
  [TD_EMOTE] = ACTION_TAP_DANCE_FN(TDEMOTE),
  [TD_QUOTL2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tdgame_finished, tdgame_reset)
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

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_GUIL0:
			if (record->event.pressed) {
        layer_clear();
        send_string(SS_TAP(X_LGUI));
			} else {
      }
		break;
  }
  return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*	Base Layer
*	,-------------------------------------------------------------------------.
*	|(esc)|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |Bspace | (ctrl+alt)/esc
*	|-------------------------------------------------------------------------+
*	|Tab/L3|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |'/2/G |  tap dance (S-') (D-game toggle) (H-Layer2)
*	|-------------------------------------------------------------------------+
*	| Shift |  Z  |  X  |  C  |  V  |  B  |  N  |M(?) |,(?!)|.(!) |emote| Ent | combo (m+,)=! combo (,+.)=? tap dance (S-"¯\_(ツ)_/¯") (D-"=)"
*	|-------------------------------------------------------------------------+
*	| Ctrl|  _  |=/Alt|  /  |   *  |   Space   |Del/L4| left| Up  |Down |Right|
*	`-------------------------------------------------------------------------'
*/
	[0] = LAYOUT(LCA_T(KC_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		LT(3,KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, TD(TD_QUOTL2),
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_EMOTE), KC_SFTENT,
		KC_LCTL, KC_UNDS, LALT_T(KC_EQL), KC_PSLS, KC_PAST, KC_SPC, LT(4,KC_DEL), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT),

/*	Game Layer
*	,-------------------------------------------------------------------------.
*	|     |  -  |  Q  |  W  |  E  |  R  |  +  |     |     |     |     |       | (ctrl+alt)/esc
*	|-------------------------------------------------------------------------+
*	|      |  G  |  A  |  S  |  D  |  F  |     |     |     |     |     |Basel |
*	|-------------------------------------------------------------------------+
*	|       |  B  |  Z  |  X  |  C  |  V  |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |           |      |     |     |     |GUIl0|
*	`-------------------------------------------------------------------------'
*/
	[1] = LAYOUT(KC_TRNS, KC_PMNS, KC_Q, KC_W, KC_E, KC_R, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_G, KC_A, KC_S, KC_D, KC_F, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TG(1),
		KC_TRNS, KC_B, KC_Z, KC_X, KC_C, KC_V, KC_TRNS, KC_M, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, M_GUIL0),

/*
*	Layer 2
*	,-------------------------------------------------------------------------.
*	| ` ~ |(np)1|(np)2|(np)3|(np)4|  5  |  6  |  7  |  8  |9([) |0(]) |       | tap dance (S-NumPad 1-4 [ ])
*	|-------------------------------------------------------------------------+
*	|      |  n5  | n6  | n7 |  n8 |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       | n9  | n0  |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       |     |     |rCtl | Gui |
*	`-------------------------------------------------------------------------'
*/
	[2] = LAYOUT(KC_GRV, TD(TD_NP1), TD(TD_NP2), TD(TD_NP3), TD(TD_NP4), KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_P5, KC_P6, KC_P7, KC_P8, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_P9, KC_P0, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RCTL, KC_RGUI),

/*
*	Layer 3
*	,-------------------------------------------------------------------------.
*	| ` ~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |9([) |0(]) |       | tap dance (S-[ ])
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |     |     |     |     |  \   |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |  @  |     |  -  |  +   |          |       | Home| PgDn| PgUp| End |
*	`-------------------------------------------------------------------------'
*/
	[3] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, TD(TD_LBRC), TD(TD_RBRC), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSLS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_AT, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END),

/*
*	Layer 4
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
/*
*	Layer 5
*	,-------------------------------------------------------------------------.
*	|     |     |     |     |     |     |     |     |     |     |     |       |
*	|-------------------------------------------------------------------------+
*	|      |     |     |     |     |     |     |     |     |     |     |      |
*	|-------------------------------------------------------------------------+
*	|       |     |     |     |     |     |     |     |     |     |     |     |
*	|-------------------------------------------------------------------------+
*	|     |     |     |     |      |          |       |     |l(1) |     |     |
*	`-------------------------------------------------------------------------'

	[5] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, M_CHAT_ENT,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TG(0), KC_TRNS, KC_TRNS),
*/
};
