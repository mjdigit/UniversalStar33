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

#ifndef UNIVERSALSTAR33_TOUCH_START_SUPPRESS_FRAMES
#    define UNIVERSALSTAR33_TOUCH_START_SUPPRESS_FRAMES 2
#endif

#ifndef UNIVERSALSTAR33_MOTION_ANOMALY_WINDOW_MS
#    define UNIVERSALSTAR33_MOTION_ANOMALY_WINDOW_MS 50
#endif

#ifndef UNIVERSALSTAR33_MOTION_ANOMALY_COUNT
#    define UNIVERSALSTAR33_MOTION_ANOMALY_COUNT 2
#endif

#ifndef UNIVERSALSTAR33_MOTION_ANOMALY_LIMIT
#    define UNIVERSALSTAR33_MOTION_ANOMALY_LIMIT 127
#endif

#ifndef UNIVERSALSTAR33_QUARANTINE_RELEASE_FRAMES
#    define UNIVERSALSTAR33_QUARANTINE_RELEASE_FRAMES 3
#endif

#ifndef UNIVERSALSTAR33_I2C_RECOVERY_ERROR_COUNT
#    define UNIVERSALSTAR33_I2C_RECOVERY_ERROR_COUNT 5
#endif

#ifndef UNIVERSALSTAR33_I2C_RECOVERY_BACKOFF_MS
#    define UNIVERSALSTAR33_I2C_RECOVERY_BACKOFF_MS 1000
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
static uint8_t              previous_finger_count;
static uint8_t              touch_start_suppress_frames;
static bool                 motion_quarantined;
static uint8_t              quarantine_release_frames;
static uint8_t              motion_anomaly_count;
static uint32_t             motion_anomaly_window_started;
static uint8_t              consecutive_i2c_errors;
static bool                 i2c_recovery_backoff;
static uint32_t             i2c_recovery_backoff_started;

static int32_t absolute_i16(int16_t value) {
    return value < 0 ? -(int32_t)value : (int32_t)value;
}

static void reset_touchpad_runtime_state(void) {
    three_finger_state          = THREE_FINGER_IDLE;
    previous_finger_count       = 0;
    touch_start_suppress_frames = 0;
    motion_quarantined          = false;
    quarantine_release_frames  = 0;
    motion_anomaly_count        = 0;
}

static void enter_motion_quarantine(void) {
    motion_quarantined         = true;
    quarantine_release_frames = 0;
    motion_anomaly_count       = 0;
}

static bool process_motion_protection(const azoteq_iqs5xx_base_data_t *base_data, int16_t x, int16_t y) {
    const uint8_t finger_count = base_data->number_of_fingers;
    bool          suppress     = false;

    if (motion_quarantined) {
        if (finger_count == 0) {
            if (quarantine_release_frames < UNIVERSALSTAR33_QUARANTINE_RELEASE_FRAMES) {
                quarantine_release_frames++;
            }
            if (quarantine_release_frames >= UNIVERSALSTAR33_QUARANTINE_RELEASE_FRAMES) {
                motion_quarantined         = false;
                quarantine_release_frames = 0;
            }
        } else {
            quarantine_release_frames = 0;
        }

        previous_finger_count = finger_count;
        return true;
    }

    const bool anomalous_motion = finger_count == 1 && (absolute_i16(x) > UNIVERSALSTAR33_MOTION_ANOMALY_LIMIT || absolute_i16(y) > UNIVERSALSTAR33_MOTION_ANOMALY_LIMIT);
    if (anomalous_motion) {
        if (motion_anomaly_count == 0 || timer_elapsed32(motion_anomaly_window_started) > UNIVERSALSTAR33_MOTION_ANOMALY_WINDOW_MS) {
            motion_anomaly_count          = 1;
            motion_anomaly_window_started = timer_read32();
        } else if (motion_anomaly_count < UINT8_MAX) {
            motion_anomaly_count++;
        }

        if (motion_anomaly_count >= UNIVERSALSTAR33_MOTION_ANOMALY_COUNT) {
            enter_motion_quarantine();
            suppress = true;
        }
    } else if (motion_anomaly_count > 0 && timer_elapsed32(motion_anomaly_window_started) > UNIVERSALSTAR33_MOTION_ANOMALY_WINDOW_MS) {
        motion_anomaly_count = 0;
    }

    if (previous_finger_count == 0 && finger_count > 0) {
        touch_start_suppress_frames = UNIVERSALSTAR33_TOUCH_START_SUPPRESS_FRAMES;
    }

    if (finger_count == 0) {
        touch_start_suppress_frames = 0;
    } else if (touch_start_suppress_frames > 0) {
        touch_start_suppress_frames--;
        suppress = true;
    }

    previous_finger_count = finger_count;
    return suppress;
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
    reset_touchpad_runtime_state();
    azoteq_iqs5xx_init();
}
#else
bool pointing_device_driver_init(void) {
    reset_touchpad_runtime_state();
    return azoteq_iqs5xx_init();
}
#endif

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    (void)mouse_report;

    report_mouse_t report = {0};

    // Do not repeatedly spend the main keyboard loop in I2C timeouts. After
    // several failures, leave enough time for key scanning and retry one read
    // after the backoff interval. A successful read clears the fault state.
    if (i2c_recovery_backoff) {
        if (timer_elapsed32(i2c_recovery_backoff_started) < UNIVERSALSTAR33_I2C_RECOVERY_BACKOFF_MS) {
            return report;
        }
        i2c_recovery_backoff  = false;
        consecutive_i2c_errors = UNIVERSALSTAR33_I2C_RECOVERY_ERROR_COUNT - 1;
    }

    azoteq_iqs5xx_base_data_t base_data = {0};
    const i2c_status_t        status    = azoteq_iqs5xx_get_base_data(&base_data);

    if (status != I2C_STATUS_SUCCESS) {
        if (consecutive_i2c_errors < UINT8_MAX) {
            consecutive_i2c_errors++;
        }

        if (consecutive_i2c_errors >= UNIVERSALSTAR33_I2C_RECOVERY_ERROR_COUNT) {
            consecutive_i2c_errors       = 0;
            i2c_recovery_backoff         = true;
            i2c_recovery_backoff_started = timer_read32();
            reset_touchpad_runtime_state();
        }
        return report;
    }

    consecutive_i2c_errors = 0;

    const int16_t x = AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.x.h, base_data.x.l);
    const int16_t y = AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.y.h, base_data.y.l);
    bool          ignore_movement = false;

    const bool suppress_motion = process_motion_protection(&base_data, x, y);

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

    if (suppress_motion) {
        report.x = 0;
        report.y = 0;
        report.h = 0;
        report.v = 0;
    }

    return report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return azoteq_iqs5xx_get_cpi();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    azoteq_iqs5xx_set_cpi(cpi);
}
