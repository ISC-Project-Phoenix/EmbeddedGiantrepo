
#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

// Phoenix CAN message IDs
#define CAN_SET_ANGLE 0x004
#define CAN_SET_SPEED 0x006
#define CAN_ENCODER   0x007

// CAN bitrate
#define CAN_BITRATE 500000

// Initialize CAN peripheral
HAL_StatusTypeDef canInit(void);

// Send CAN message
HAL_StatusTypeDef canSend(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
);

// Process incoming CAN messages
void canProcessReceived(void);

// Handle a received CAN message
void canHandleMessage(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
);
