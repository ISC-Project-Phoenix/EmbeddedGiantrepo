#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

// Steering angle limits from original Phoenix code
#define MAX_STEERING_ANGLE 24.0f

// Initialize steering hardware
HAL_StatusTypeDef steeringInit(void);

// Request a steering angle in degrees
bool steeringSetAngle(float angle);

// Return current requested angle
float steeringGetAngle(void);

// Disable steering commands
void steeringDisable(void);
