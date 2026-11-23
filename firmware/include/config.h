#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

// CAN message IDs from Project Phoenix
#define CAN_SET_ANGLE  0x004
#define CAN_SET_SPEED  0x006
#define CAN_ENCODER    0x007

// CAN communication speed
#define CAN_BITRATE 500000

// Initialize the CAN peripheral
HAL_StatusTypeDef canInit(void);

// Send a CAN message
HAL_StatusTypeDef canSend(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
);

// Check and process incoming messages
void canProcessReceived(void);

// Handle received CAN messages
void canHandleMessage(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
);
