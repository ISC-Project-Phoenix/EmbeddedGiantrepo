#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

// Initialize PWM hardware
HAL_StatusTypeDef pwmInit(void);

// Start PWM on a timer channel
HAL_StatusTypeDef pwmStart(
    TIM_HandleTypeDef* timer,
    uint32_t channel
);

// Stop PWM on a timer channel
HAL_StatusTypeDef pwmStop(
    TIM_HandleTypeDef* timer,
    uint32_t channel
);

// Set timer compare value
HAL_StatusTypeDef pwmSetCompare(
    TIM_HandleTypeDef* timer,
    uint32_t channel,
    uint32_t compareValue
);
