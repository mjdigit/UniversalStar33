// Copyright 2026 mjdigit
// SPDX-License-Identifier: GPL-2.0-or-later

#include "drivers/sensors/azoteq_iqs5xx.h"
#include "pointing_device_internal.h"
#include "timer.h"

#ifndef UNIVERSALSTAR33_THREE_FINGER_TAP_TIME
#    define UNIVERSALSTAR33_THREE_FINGER_TAP_TIME 250
#endif

#ifndef UNIVERSALSTAR33_THREE_FINGER_RELEASE_TIME
#    define UNIVERSALSTAR33_THREE_FINGER_RELEASE_TIME 80
#endif

#ifndef UNIVERSALSTAR33_THREE_FINGER_TAP_DISTANCE
#    define UNIVERSALSTAR33_THREE_FINGER_TAP_DISTANCE 50
#endif

typedef enum {
    THREE_FINGER_IDLE,
    THREE_FINGER_TRACKING,
    THREE_FINGER_RELEASING,
    THREE_FINGER_CANCELLED,
} three_finger_state_t;

static three_finger_state_t three_finger_state;
static uint32_t             three_finger_started;
static uint32_t             three_finger_release_started;
static uint16_t             three_finger_movement;

static int32_t absolute_i16(int16_t value) {
    return value < 0 ? -(int32_t)value : (int32_t)value;
}

static bool process_three_finger_tap(uint8_t finger_count, int16_t x, int16_t y, report_mouse_t *mouse_report) {
    bool suppress_native_gestures = three_finger_state != THREE_FINGER_IDLE;

    switch (three_finger_state) {
        case THREE_FINGER_IDLE:
            if (finger_count >= 3) {
                three_finger_state    = THREE_FINGER_TRACKING;
                three_finger_started  = timer_read32();
                three_finger_movement = 0;
                suppress_native_gestures = true;
            }
            break;

        case THREE_FINGER_TRACKING:
            if (timer_elapsed32(three_finger_started) > UNIVERSALSTAR33_THREE_FINGER_TAP_TIME) {
                three_finger_state = THREE_FINGER_CANCELLED;
            } else if (finger_count >= 3) {
                uint32_t movement = (uint32_t)three_finger_movement + absolute_i16(x) + absolute_i16(y);
                three_finger_movement = movement > UINT16_MAX ? UINT16_MAX : (uint16_t)movement;
                if (three_finger_movement > UNIVERSALSTAR33_THREE_FINGER_TAP_DISTANCE) {
                    three_finger_state = THREE_FINGER_CANCELLED;
                }
            } else if (finger_count == 0) {
                mouse_report->buttons |= 1 << POINTING_DEVICE_BUTTON3;
                three_finger_state = THREE_FINGER_IDLE;
            } else {
                // Fingers may leave the pad one at a time. Allow a short
                // release interval before deciding that the gesture ended.
                three_finger_state           = THREE_FINGER_RELEASING;
                three_finger_release_started = timer_read32();
            }
            break;

        case THREE_FINGER_RELEASING:
            if (finger_count == 0 && timer_elapsed32(three_finger_started) <= UNIVERSALSTAR33_THREE_FINGER_TAP_TIME) {
                mouse_report->buttons |= 1 << POINTING_DEVICE_BUTTON3;
                three_finger_state = THREE_FINGER_IDLE;
            } else if (finger_count >= 3) {
                three_finger_state = THREE_FINGER_TRACKING;
            } else if (timer_elapsed32(three_finger_release_started) > UNIVERSALSTAR33_THREE_FINGER_RELEASE_TIME ||
                       timer_elapsed32(three_finger_started) > UNIVERSALSTAR33_THREE_FINGER_TAP_TIME) {
                three_finger_state = THREE_FINGER_CANCELLED;
            }
            break;

        case THREE_FINGER_CANCELLED:
            if (finger_count == 0) {
                three_finger_state = THREE_FINGER_IDLE;
            }
            break;
    }

    return suppress_native_gestures;
}

#ifdef VIAL_ENABLE
void pointing_device_driver_init(void) {
    three_finger_state = THREE_FINGER_IDLE;
    azoteq_iqs5xx_init();
}
#else
bool pointing_device_driver_init(void) {
    three_finger_state = THREE_FINGER_IDLE;
    return azoteq_iqs5xx_init();
}
#endif

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    (void)mouse_report;

    report_mouse_t             report    = {0};
    azoteq_iqs5xx_base_data_t  base_data = {0};
    const i2c_status_t         status    = azoteq_iqs5xx_get_base_data(&base_data);

    if (status != I2C_STATUS_SUCCESS) {
        return report;
    }

    const int16_t x = AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.x.h, base_data.x.l);
    const int16_t y = AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.y.h, base_data.y.l);
    bool          ignore_movement = false;

    const bool suppress_native_gestures = process_three_finger_tap(base_data.number_of_fingers, x, y, &report);

    if (!suppress_native_gestures) {
        if (base_data.gesture_events_0.single_tap || base_data.gesture_events_0.press_and_hold) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON1);
        } else if (base_data.gesture_events_1.two_finger_tap) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON2);
        } else if (base_data.gesture_events_0.swipe_x_neg) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON4);
            ignore_movement = true;
        } else if (base_data.gesture_events_0.swipe_x_pos) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON5);
            ignore_movement = true;
        } else if (base_data.gesture_events_0.swipe_y_neg) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON6);
            ignore_movement = true;
        } else if (base_data.gesture_events_0.swipe_y_pos) {
            report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON3);
            ignore_movement = true;
        } else if (base_data.gesture_events_1.zoom) {
            if (x < 0) {
                report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON7);
            } else if (x > 0) {
                report.buttons = pointing_device_handle_buttons(report.buttons, true, POINTING_DEVICE_BUTTON8);
            }
        } else if (base_data.gesture_events_1.scroll) {
            report.h = CONSTRAIN_HID(x);
            report.v = CONSTRAIN_HID(y);
        }

        if (base_data.number_of_fingers == 1 && !ignore_movement) {
            report.x = CONSTRAIN_HID_XY(x);
            report.y = CONSTRAIN_HID_XY(y);
        }
    }

    return report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return azoteq_iqs5xx_get_cpi();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    azoteq_iqs5xx_set_cpi(cpi);
}
