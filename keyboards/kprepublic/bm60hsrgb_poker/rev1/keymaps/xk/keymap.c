#include QMK_KEYBOARD_H

/*
 * 14, 14, 13, 12, 8
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_60_ansi(
      KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
      LT(1,KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
      KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
      KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, MO(1), KC_RGUI, KC_RCTL),
  [1] = LAYOUT_60_ansi(
      KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
      KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO, KC_NO, KC_HOME, KC_NO, KC_NO, KC_END, KC_PSCR, KC_PGDN, KC_PGUP, KC_NO,
      KC_TRNS, KC_VOLD, KC_MUTE, KC_VOLU, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_NO, KC_NO, KC_NO,
      KC_NO, RGB_VAD, RGB_TOG, RGB_VAI, RGB_M_P, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, MO(2), KC_NO, KC_TRNS, KC_NO, KC_TRNS, MO(2), KC_NO),
  [2] = LAYOUT_60_ansi(
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
      KC_NO, RGB_MOD, RGB_HUI, RGB_SAI, RGB_SPI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_SPD, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO),
  [3] = LAYOUT_60_ansi(
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO)
};

static uint16_t timeout_threshold = 5;  // timeout in minutes
static uint16_t timeout_timer = 0;
static uint16_t timeout_counter = 0;    // in minute intervals

void timeout_reset_timer(void) {
  timeout_timer = timer_read();
  timeout_counter = 0;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    default:
      if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_enable();
#endif
        timeout_reset_timer();
      }
      break;
  }
  return true;
};

void matrix_scan_user(void) {
  if (timer_elapsed(timeout_timer) >= 60000) { // 1 minute tick
    timeout_counter++;
    timeout_timer = timer_read();
  }
#ifdef RGB_MATRIX_ENABLE
  if (timeout_counter >= timeout_threshold) {
    rgb_matrix_disable_noeeprom();
  }
#endif
}

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  switch(get_highest_layer(layer_state)){  // special handling per layer
    case 0:
      // don't change default behavior
      break;
    case 1:
      rgb_matrix_set_color(0, RGB_GOLD);
      break;
    case 2:
      rgb_matrix_set_color(0, RGB_GREEN);
      break;
    case 3:
      rgb_matrix_set_color(0, RGB_RED);
      break;
    default:
      break;
  }
}

void suspend_power_down_user(void) {
  rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_user(void) {
  rgb_matrix_set_suspend_state(false);
}
#endif
