#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

// Initialize throttle hardware
HAL_StatusTypeDef throttleInit(void);

// Request normalized throttle (0.0 to 1.0)
bool throttleSet(float value);

// Get current requested throttle
float throttleGet(void);

// Disable throttle output
void throttleDisable(void);
