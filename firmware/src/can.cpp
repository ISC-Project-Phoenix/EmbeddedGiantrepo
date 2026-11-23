#include "can.h"

// CAN peripheral handle
static CAN_HandleTypeDef hcan;

// 1. Initialize CAN
HAL_StatusTypeDef canInit(void) {

    // Enable CAN peripheral clock

    // Enable GPIO clock

    // Configure CAN TX and RX pins
    // Use verified STM32F303K8 pin assignments

    // Set up CAN parameters
    hcan.Instance = CAN;

    // Configure:
    // Prescaler
    // Operating mode
    // Synchronization jump width
    // Time segment 1
    // Time segment 2
    // Automatic retransmission

    // Initialize CAN using HAL

    // Configure CAN receive filters

    // Start CAN peripheral

    // Return HAL_OK when initialization succeeds

    return HAL_ERROR; // Placeholder until implemented
}


// 2. Transmit a CAN message
HAL_StatusTypeDef canSend(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
) {

    // Check for invalid data pointer

    // Check that length is between 0 and 8

    // Create CAN_TxHeaderTypeDef

    // Set CAN message ID (extended format)

    // Set message length

    // Check if transmit mailbox is available

    // Send message using HAL_CAN_AddTxMessage()

    return HAL_ERROR; // Placeholder
}


// 3. Process incoming CAN messages
void canProcessReceived(void) {

    // Check CAN receive FIFO

    // Read the message header and data

    // Check message ID and length

    // Pass message to canHandleMessage()

}


// 4. Handle CAN messages
void canHandleMessage(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
) {

    // Check incoming message type

    if (id == CAN_SET_ANGLE) {

        // Validate steering message
        // Decode steering data
        // Pass to controller

    }
    else if (id == CAN_SET_SPEED) {

        // Validate speed message
        // Decode speed data
        // Pass to controller

    }
    else if (id == CAN_ENCODER) {

        // Validate encoder message
        // Decode encoder data

    }
}
