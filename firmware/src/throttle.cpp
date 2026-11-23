#include "throttle.h"

static float requestedThrottle = 0.0f;

HAL_StatusTypeDef throttleInit(void) {
    requestedThrottle = 0.0f;
    return HAL_ERROR;
}

bool throttleSet(float value) {
    if (value != value) {
        return false;
    }

    if (value < 0.0f || value > 1.0f) {
        return false;
    }

    requestedThrottle = value;
    return true;
}

float throttleGet(void) {
    return requestedThrottle;
}

void throttleDisable(void) {
    requestedThrottle = 0.0f;
}