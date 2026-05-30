#include QMK_KEYBOARD_H

enum layers {
    CLK,
    NUM,
    NAV,
    SYM,
    FUN,
    MOU,
    MED,
    SYR,
    SYL,
    BPD,
};

// --- TAP DANCE ---
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }
    return TD_UNKNOWN;
}

// --- TAP DANCE BOOT ---
static td_tap_t td_boot_state = { .is_press_action = true, .state = TD_NONE };

void td_boot_finished(tap_dance_state_t *state, void *user_data) {
    td_boot_state.state = cur_dance(state);
    switch (td_boot_state.state) {
        case TD_SINGLE_TAP:  break;
        case TD_SINGLE_HOLD: break;
        case TD_DOUBLE_TAP:  reset_keyboard();  break;
        case TD_DOUBLE_HOLD: soft_reset_keyboard(); break;
        default: break;
    }
}

void td_boot_reset(tap_dance_state_t *state, void *user_data) {
    td_boot_state.state = TD_NONE;
}

// --- TAP DANCE PASTE ---
static td_tap_t td_paste_state = { .is_press_action = true, .state = TD_NONE };

void td_paste_finished(tap_dance_state_t *state, void *user_data) {
    td_paste_state.state = cur_dance(state);
    switch (td_paste_state.state) {
        case TD_SINGLE_TAP: tap_code(KC_PASTE); break;
        case TD_SINGLE_HOLD: tap_code16(LCTL(KC_V)); break;
        case TD_DOUBLE_TAP:  tap_code16(LSFT(LCTL(KC_V))); break;
        case TD_DOUBLE_HOLD: break;
        default: break;
    }
}

void td_paste_reset(tap_dance_state_t *state, void *user_data) {
    td_paste_state.state = TD_NONE;
}

// --- TAP DANCE COPY ---
static td_tap_t td_copy_state = { .is_press_action = true, .state = TD_NONE };

void td_copy_finished(tap_dance_state_t *state, void *user_data) {
    td_copy_state.state = cur_dance(state);
    switch (td_copy_state.state) {
        case TD_SINGLE_TAP: tap_code(KC_COPY); break;
        case TD_SINGLE_HOLD: tap_code16(LCTL(KC_C)); break;
        case TD_DOUBLE_TAP:  tap_code16(LSFT(LCTL(KC_C))); break;
        case TD_DOUBLE_HOLD: break;
        default: break;
    }
}

void td_copy_reset(tap_dance_state_t *state, void *user_data) {
    td_copy_state.state = TD_NONE;
}

// --- TAP DANCE CODES ---
enum tap_dance_codes {
    BOOT,
    PASTE,
    COPY,
};

tap_dance_action_t tap_dance_actions[] = {
    [BOOT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_boot_finished, td_boot_reset),
    [PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_paste_finished, td_paste_reset),
    [COPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_copy_finished, td_copy_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [CLK] = LAYOUT_split_3x5_2(
        RALT_T(KC_Q), LT(SYR,KC_W), LCTL_T(KC_F), LSFT_T(KC_P), KC_G,     KC_J, RSFT_T(KC_L), LCTL_T(KC_U), LT(SYL,KC_Y), RALT_T(KC_SCLN),
        LGUI_T(KC_A), LALT_T(KC_R), LT(NUM,KC_S), LT(NAV,KC_T), KC_D,     KC_H, LT(SYM,KC_N), LT(FUN,KC_E), LALT_T(KC_I), LGUI_T(KC_O),
        KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,     KC_K, KC_M,         KC_COMM,      KC_DOT,       KC_SLSH,
                                      LT(BPD,KC_ESC), LT(MED,KC_ENT),     LT(MOU,KC_TAB), KC_SPC
    ),

    [NUM] = LAYOUT_split_3x5_2(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_7, KC_8, KC_9, KC_COMM,
        KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS, KC_TRNS,     KC_TRNS, KC_4, KC_5, KC_6, KC_0,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_1, KC_2, KC_3, KC_TRNS,
                            KC_TRNS, LCTL_T(KC_ENT),     KC_TRNS, KC_TRNS
    ),

    [NAV] = LAYOUT_split_3x5_2(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS,     KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_CAPS, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_INS,  KC_PSCR, KC_APP,  KC_TRNS,
                            KC_TRNS, LSFT_T(KC_ENT),     KC_TRNS, KC_TRNS
    ),

    [SYM] = LAYOUT_split_3x5_2(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_LPRN, KC_RBRC, KC_NUHS, KC_RPRN, CW_TOGG,     KC_TRNS, KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,
        KC_RCBR, KC_LT,   KC_GT,   KC_PIPE, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                   KC_TRNS, KC_TRNS,     LSFT_T(KC_TAB), KC_TRNS
    ),

    [FUN] = LAYOUT_split_3x5_2(
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_TRNS,     KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS, KC_TRNS,
        KC_F9,  KC_F10, KC_F11, KC_F12, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                               KC_TRNS, KC_TRNS,     LCTL_T(KC_TAB), KC_TRNS
    ),

    [MOU] = LAYOUT_split_3x5_2(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, MS_WHLL, MS_WHLU, MS_WHLD, MS_WHLR,
        MS_BTN4, MS_BTN2, MS_BTN1, MS_BTN5, KC_TRNS,     KC_TRNS, MS_LEFT, MS_UP,   MS_DOWN, MS_RGHT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, MS_BTN4, MS_BTN1, MS_BTN2, MS_BTN5,
                                   KC_TRNS, KC_TRNS,     KC_NO,   KC_TRNS
    ),

    [MED] = LAYOUT_split_3x5_2(
        KC_LALT,    KC_TAB,     KC_LCTL,  KC_TAB,    KC_TRNS,       KC_TRNS, KC_MPLY, KC_VOLU, KC_VOLD, KC_TRNS,
        LCTL(KC_Z), LCTL(KC_X), TD(COPY), TD(PASTE), KC_TRNS,       KC_TRNS, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT,
        LCTL(KC_Y), KC_TRNS,    KC_TRNS,  KC_TRNS,   KC_TRNS,       KC_TRNS, KC_MUTE, KC_MPRV, KC_MNXT, KC_TRNS,
                                          TD(BOOT),  KC_NO,         LSFT(KC_TAB), KC_SPC
    ),

    [SYR] = LAYOUT_split_3x5_2(
        KC_TRNS, KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_LCBR, KC_LBRC,      KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_DQUO, KC_QUOT, KC_INT1, LSFT(KC_INT1),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_PERC, KC_AMPR, KC_ASTR,      KC_QUES,
                                   KC_TRNS, LALT_T(KC_ENT),      KC_TRNS, KC_TRNS
    ),

    [SYL] = LAYOUT_split_3x5_2(
        KC_TRNS,       KC_GRV,  KC_TILD, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS,
        LSFT(KC_NUBS), KC_NUBS, KC_MINS, KC_UNDS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,       KC_EQL,  KC_PPLS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                         KC_TRNS, KC_TRNS,     LALT_T(KC_TAB), KC_TRNS
    ),

    [BPD] = LAYOUT_split_3x5_2(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                   KC_NO,   KC_TRNS,     KC_DEL,  KC_BSPC
    ),
};

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(BPD,KC_ESC):
        case LT(MED,KC_ENT):
        case LT(MOU,KC_TAB):
        case LSFT_T(KC_ENT):
        case LCTL_T(KC_ENT):
        case LALT_T(KC_ENT):
        case LSFT_T(KC_TAB):
        case LCTL_T(KC_TAB):
        case LALT_T(KC_TAB):
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RALT_T(KC_Q):
        case RALT_T(KC_SCLN):
        case LCTL_T(KC_F):
        case LCTL_T(KC_U):
        case LGUI_T(KC_A):
        case LGUI_T(KC_O):
        case LSFT_T(KC_P):
        case RSFT_T(KC_L):
        case LALT_T(KC_I):
        case LALT_T(KC_R):
        case LT(SYR,KC_W):
        case LT(SYL,KC_Y):
        case LT(NUM,KC_S):
        case LT(NAV,KC_T):
        case LT(SYM,KC_N):
        case LT(FUN,KC_E):
            return true;
        default:
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(BPD,KC_ESC):
        case LT(MED,KC_ENT):
        case LT(MOU,KC_TAB):
        case LALT_T(KC_ENT):
        case LCTL_T(KC_ENT):
        case LSFT_T(KC_ENT):
        case LSFT_T(KC_TAB):
        case LCTL_T(KC_TAB):
        case LALT_T(KC_TAB):
             return true;
        default:
            return false;
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(SYR,KC_W):
        case LT(SYL,KC_Y):
        case LSFT_T(KC_P):
        case RSFT_T(KC_L):
            return 130;
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    switch (keycode) {
        case RALT_T(KC_Q):
        case LT(SYR,KC_W):
        case LCTL_T(KC_F):
        case LSFT_T(KC_P):
        case RSFT_T(KC_L):
        case LCTL_T(KC_U):
        case LT(SYL,KC_Y):
        case RALT_T(KC_SCLN):
        case LGUI_T(KC_A):
        case LALT_T(KC_R):
        case LT(NUM,KC_S):
        case LT(NAV,KC_T):
        case LT(SYM,KC_N):
        case LT(FUN,KC_E):
        case LALT_T(KC_I):
        case LGUI_T(KC_O):
        case LSFT_T(KC_ENT):
        case LALT_T(KC_ENT):
        case LCTL_T(KC_ENT):
        case LSFT_T(KC_TAB):
        case LCTL_T(KC_TAB):
        case LALT_T(KC_TAB):
        case LT(MED,KC_ENT):
        case LT(MOU,KC_TAB):
        case LT(BPD,KC_ESC):
            return 0;
        default:
            return FLOW_TAP_TERM;
    }
}
