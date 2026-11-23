#pragma once

// Shared receiver / safety constants.
// All pulse widths are in microseconds (standard RC servo PWM).

#define PWM_MIN        1000
#define PWM_NEUTRAL    1500
#define PWM_MAX        2000

// Switches must be at or above this width to count as "on"
#define DEADMAN_MIN    1700
#define TELEOP_MIN     1700

// Receiver counts as disconnected if any channel is older than this (ms)
#define SIGNAL_TIMEOUT_MS 100
