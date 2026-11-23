#include "pwm.h"

HAL_StatusTypeDef pwmInit(void) {
    // TODO: configure the output timer(s) and pins once the steering and
    // throttle outputs are chosen. Returns an error until then.
    return HAL_ERROR;
}

HAL_StatusTypeDef pwmStart(
    TIM_HandleTypeDef* timer,
    uint32_t channel
) {
    if (timer == nullptr) {
        return HAL_ERROR;
    }

    return HAL_TIM_PWM_Start(timer, channel);
}

HAL_StatusTypeDef pwmStop(
    TIM_HandleTypeDef* timer,
    uint32_t channel
) {
    if (timer == nullptr) {
        return HAL_ERROR;
    }

    return HAL_TIM_PWM_Stop(timer, channel);
}

HAL_StatusTypeDef pwmSetCompare(
    TIM_HandleTypeDef* timer,
    uint32_t channel,
    uint32_t compareValue
) {
    if (timer == nullptr) {
        return HAL_ERROR;
    }

    if (compareValue > __HAL_TIM_GET_AUTORELOAD(timer)) {
        return HAL_ERROR;
    }

    __HAL_TIM_SET_COMPARE(timer, channel, compareValue);
    return HAL_OK;
}