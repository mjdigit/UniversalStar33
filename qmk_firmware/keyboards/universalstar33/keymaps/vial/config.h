// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID {0xC0, 0xA9, 0x34, 0xC1, 0x10, 0x1A, 0xD4, 0x9A}

// Hold the physical top-left and top-right keys to unlock Vial.
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 9}

#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define VIAL_TAP_DANCE_ENTRIES 32
#define VIAL_COMBO_ENTRIES 32
#define VIAL_KEY_OVERRIDE_ENTRIES 32

#define TAPPING_TERM 120
#define PERMISSIVE_HOLD
