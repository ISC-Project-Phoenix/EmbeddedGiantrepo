#include "steering.h"

static float requestedAngle = 0.0f;

HAL_StatusTypeDef steeringInit(void) {
    requestedAngle = 0.0f;
    return HAL_ERROR;
}

bool steeringSetAngle(float angle) {
    if (angle != angle) {
        return false;
    }

    if (angle < -MAX_STEERING_ANGLE ||
        angle > MAX_STEERING_ANGLE) {
        return false;
    }

    requestedAngle = angle;
    return true;
}

float steeringGetAngle(void) {
    return requestedAngle;
}

void steeringDisable(void) {
    requestedAngle = 0.0f;
}