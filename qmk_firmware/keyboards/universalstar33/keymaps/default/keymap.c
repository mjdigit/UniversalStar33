// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "drivers/sensors/azoteq_iqs5xx.h"

enum custom_keycodes {
  DRGSCRL = QK_KB_0, // Set drag scroll mode
  CPIPLS, // Increase CPI for 100.
  CPIMNS, // Decrease CPI for 100.
};

enum keymap_layers {
  _BASE0, // Windows Layers
  _LOWER0,
  _UPPER0,
  _SPEC0,
  _BASE1, // Option Layers
  _LOWER1,
  _UPPER1,
  _SPEC1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE0] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                                ---------------|---------------|---------------|---------------|---------------|
    LGUI_T(KC_Q)        , KC_W          , KC_E          , KC_R          , KC_T                                                          , KC_Y          , KC_U          , KC_I          , KC_O          , RGUI_T(KC_P)  ,
    //    ---------------|---------------|---------------|---------------|---------------|                                                ---------------|---------------|---------------|---------------|---------------|
    KC_A                , KC_S          , KC_D          , KC_F          , KC_G                                                          , KC_H          , KC_J          , KC_K          , KC_L          , KC_ENT        ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    LCTL_T(KC_Z)        , LSFT_T(KC_X)  , LALT_T(KC_C)  , KC_V          , LT(1, KC_B)   , KC_SPC        , MS_BTN1       , MS_BTN2       , LT(2, KC_N)   , KC_M          , RALT_T(KC_COMM), RSFT_T(KC_DOT), RCTL_T(KC_SLSH)
  ),
  [_LOWER0] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    LGUI_T(KC_F2)       , KC_F3         , KC_F4         , KC_F5         , KC_F6                                               , KC_F7         , KC_F8         , KC_F9         , KC_F10        , RGUI_T(KC_F11),
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_1                , KC_2          , KC_3          , KC_4          , KC_5                                                , KC_6          , KC_7          , KC_8          , KC_9          , KC_0          ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_LCTL             , KC_LSFT       , KC_LALT       , KC_NO         , KC_TRNS       , KC_TRNS       , KC_TRNS       , MS_BTN3       , KC_GRV        , KC_QUOT       , RALT_T(KC_LBRC), RSFT_T(KC_RBRC), RCTL_T(KC_BSLS)
  ),
  [_UPPER0] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_F13              , KC_F14        , KC_F15        , CPIPLS        , KC_NO                                                , KC_HOME       , KC_PGUP       , KC_PSCR       , KC_SCRL       , KC_PAUS       ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_F16              , KC_F17        , KC_F18        , DRGSCRL       , KC_END                                               , KC_LEFT       , KC_DOWN       , KC_UP         , KC_RGHT       , KC_BSPC       ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_LCTL             , KC_LSFT       , KC_LALT       , CPIMNS        , KC_TRNS       , LALT(KC_GRAVE), KC_NO          , KC_NO          , KC_TRNS       , KC_PGDN       , KC_DEL        , KC_APP        , KC_INS
  ),
  [_SPEC0] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO
  ),
  [_BASE1] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO
  ),
  [_LOWER1] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO
  ),
  [_UPPER1] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO
  ),
  [_SPEC1] = LAYOUT(
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|                                      ---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO                                                , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO         ,
    //    ---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
    KC_NO               , KC_NO         , KC_NO         , KC_NO         , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO         , KC_NO          , KC_NO          , KC_NO          , KC_NO
  ),
};

#ifndef VIAL_ENABLE
const uint16_t PROGMEM combo_Q_W[] = {LGUI_T(KC_Q), KC_W, COMBO_END};
const uint16_t PROGMEM combo_S_D[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM combo_O_P[] = {KC_O, RGUI_T(KC_P), COMBO_END};
const uint16_t PROGMEM combo_L_ENT[] = {KC_L, KC_ENT, COMBO_END};
const uint16_t PROGMEM combo_F2_F3[] = {LGUI_T(KC_F2), KC_F3, COMBO_END};
const uint16_t PROGMEM combo_F10_F11[] = {KC_F10, RGUI_T(KC_F11), COMBO_END};
const uint16_t PROGMEM combo_9_0[] = {KC_9, KC_0, COMBO_END};

combo_t key_combos[] = {
  COMBO(combo_Q_W, KC_ESC),
  COMBO(combo_S_D, KC_TAB),
  COMBO(combo_O_P, KC_MINS),
  COMBO(combo_L_ENT, KC_SCLN),
  COMBO(combo_F2_F3, KC_F1),
  COMBO(combo_F10_F11, KC_F12),
  COMBO(combo_9_0, KC_EQL),
};
#endif

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
#if CONSOLE_ENABLE
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
#endif
}

#ifndef VIAL_ENABLE
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case RCTL_T(KC_SLSH):
    case RSFT_T(KC_DOT):
    case RSFT_T(KC_RBRC):
    case RCTL_T(KC_BSLS):
    case LSFT_T(KC_X):
    case LCTL_T(KC_Z):
    case LSFT_T(KC_LEFT):
    case LCTL_T(KC_BSPC):
      return TAPPING_TERM + 100;
    default:
      return TAPPING_TERM;
  }
}
#endif

#define SCROLL_DEVISOR 20.0
static float scroll_accumulated_h = 0;
static float scroll_accumulated_v = 0;

static bool drag_scrolling = false;

// The IQS5xx reports both a tap and a press-and-hold as mouse button 1.
// Delay the first report by one pointing-device cycle so a one-shot tap can
// be distinguished from a continuing hold. A hold is only passed through
// when it follows a tap within the double-tap window (tap-and-drag).
#    define TOUCHPAD_DOUBLE_TAP_TERM 500

typedef enum {
  TOUCHPAD_BUTTON_IDLE,
  TOUCHPAD_BUTTON_TAP_CANDIDATE,
  TOUCHPAD_BUTTON_HOLD_SUPPRESSED,
  TOUCHPAD_BUTTON_DRAGGING,
} touchpad_button_state_t;

static touchpad_button_state_t touchpad_button_state = TOUCHPAD_BUTTON_IDLE;
static uint32_t                touchpad_last_tap_time = 0;
static bool                    touchpad_tap_armed     = false;

static void process_touchpad_tap_and_drag(report_mouse_t *mouse_report) {
  const uint8_t button1_mask = 1 << POINTING_DEVICE_BUTTON1;
  const bool    button1_raw  = (mouse_report->buttons & button1_mask) != 0;

  // Rebuild button 1 below without modifying the other mouse buttons.
  mouse_report->buttons &= ~button1_mask;

  if (touchpad_tap_armed && timer_elapsed32(touchpad_last_tap_time) > TOUCHPAD_DOUBLE_TAP_TERM) {
    touchpad_tap_armed = false;
  }

  switch (touchpad_button_state) {
    case TOUCHPAD_BUTTON_IDLE:
      if (button1_raw) {
        if (touchpad_tap_armed) {
          // The second tap may be released immediately (double-click) or
          // remain active (tap-and-drag). In either case, pass it through.
          touchpad_button_state = TOUCHPAD_BUTTON_DRAGGING;
          touchpad_tap_armed    = false;
          mouse_report->buttons |= button1_mask;
        } else {
          // Wait one report to determine whether this is a tap or a hold.
          touchpad_button_state = TOUCHPAD_BUTTON_TAP_CANDIDATE;
        }
      }
      break;

    case TOUCHPAD_BUTTON_TAP_CANDIDATE:
      if (button1_raw) {
        // A continuing event is a standalone press-and-hold. Do not turn it
        // into a drag unless it was preceded by a tap.
        touchpad_button_state = TOUCHPAD_BUTTON_HOLD_SUPPRESSED;
      } else {
        // A one-report event is a single tap. Emit its click now and arm the
        // following tap for double-click or tap-and-drag.
        mouse_report->buttons |= button1_mask;
        touchpad_last_tap_time = timer_read32();
        touchpad_tap_armed     = true;
        touchpad_button_state  = TOUCHPAD_BUTTON_IDLE;
      }
      break;

    case TOUCHPAD_BUTTON_HOLD_SUPPRESSED:
      if (!button1_raw) {
        touchpad_button_state = TOUCHPAD_BUTTON_IDLE;
      }
      break;

    case TOUCHPAD_BUTTON_DRAGGING:
      if (button1_raw) {
        mouse_report->buttons |= button1_mask;
      } else {
        touchpad_button_state = TOUCHPAD_BUTTON_IDLE;
      }
      break;
  }
}
#define PROCESS_OVERRIDE_BEHAVIOR false
#define PROCESS_USUAL_BEHAVIOR    true
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint16_t cpi;

  switch (keycode) {
  case DRGSCRL:
    drag_scrolling = record->event.pressed? true: false;
    break;
  case CPIPLS:
    cpi = azoteq_iqs5xx_get_cpi();
    cpi = (cpi + 100 > 2000)? 2000: cpi + 100;
    azoteq_iqs5xx_set_cpi(cpi);
    break;
  case CPIMNS:
    cpi = azoteq_iqs5xx_get_cpi();
    cpi = (cpi < 100)? 0: cpi - 100;
    azoteq_iqs5xx_set_cpi(cpi);
    break;
  default:
    break;
  }

  return PROCESS_USUAL_BEHAVIOR;
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
  process_touchpad_tap_and_drag(&mouse_report);

  int16_t cur_x = mouse_report.x;
  int16_t cur_y = mouse_report.y;
  int16_t cur_h = mouse_report.h;
  int16_t cur_v = mouse_report.v;

  if (drag_scrolling) {
    scroll_accumulated_h += (float)cur_x / SCROLL_DEVISOR;
    scroll_accumulated_v += (float)cur_y / SCROLL_DEVISOR;
    cur_x = 0;
    cur_y = 0;
  } else {
    scroll_accumulated_h += (float)cur_h / SCROLL_DEVISOR;
    scroll_accumulated_v += (float)cur_v / SCROLL_DEVISOR;
  }

  mouse_report.x = cur_x;
  mouse_report.y = cur_y;
  mouse_report.h = (int8_t)scroll_accumulated_h;
  mouse_report.v = (int8_t)scroll_accumulated_v;

  scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
  scroll_accumulated_v -= (int8_t)scroll_accumulated_v;
  
  return mouse_report;
}
