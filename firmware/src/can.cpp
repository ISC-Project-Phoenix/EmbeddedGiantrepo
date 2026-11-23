#include "can.h"
#include <string.h>

static CAN_HandleTypeDef hcan;
static bool canReady = false;

static uint8_t lastAngleData[8] = {};
static uint8_t lastSpeedData[8] = {};
static uint8_t lastEncoderData[8] = {};

static uint8_t lastAngleLength = 0;
static uint8_t lastSpeedLength = 0;
static uint8_t lastEncoderLength = 0;

extern "C" void HAL_CAN_MspInit(CAN_HandleTypeDef* handle) {
    if (handle == nullptr || handle->Instance != CAN) {
        return;
    }// guard to make sure it runs only with can 

    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {};

    gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF9_CAN;
    // pa11 is rx and pa12 is tx
    HAL_GPIO_Init(GPIOA, &gpio);
}

HAL_StatusTypeDef canInit(void) {
    canReady = false;

    hcan.Instance = CAN;
    hcan.Init.Prescaler = 4; // t = 125ns = 4 / 32MHz = 125,  N = 16, baud = 500kbps = 1 / (t*N)
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_2TQ;

    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = ENABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = ENABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        return HAL_ERROR;
    }

    CAN_FilterTypeDef filter = {};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        return HAL_ERROR;
    }

    canReady = true;
    return HAL_OK;
}

HAL_StatusTypeDef canSend( uint32_t id, const uint8_t* data,  uint8_t length) {
    if (!canReady || length > 8 || id > 0x7FF) {
        return HAL_ERROR;
    }

    if (length > 0 && data == nullptr) {
        return HAL_ERROR;
    }

    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        return HAL_BUSY;
    }

    CAN_TxHeaderTypeDef txHeader = {};

    txHeader.StdId = id;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = length;
    txHeader.TransmitGlobalTime = DISABLE;

    uint8_t payload[8] = {};

    if (length > 0) {
        memcpy(payload, data, length);
    }

    uint32_t mailbox = 0;

    return HAL_CAN_AddTxMessage(
        &hcan,
        &txHeader,
        payload,
        &mailbox
    );
}

void canProcessReceived(void) {
    if (!canReady) {
        return;
    }

    uint8_t processed = 0;

    while (HAL_CAN_GetRxFifoFillLevel(
        &hcan, CAN_RX_FIFO0
    ) > 0 && processed < 8) {

        CAN_RxHeaderTypeDef rxHeader = {};
        uint8_t data[8] = {};

        if (HAL_CAN_GetRxMessage(
            &hcan,
            CAN_RX_FIFO0,
            &rxHeader,
            data
        ) != HAL_OK) {
            break;
        }

        processed++;

        if (rxHeader.IDE != CAN_ID_STD ||
            rxHeader.RTR != CAN_RTR_DATA ||
            rxHeader.DLC > 8) {
            continue;
        }

        canHandleMessage(
            rxHeader.StdId,
            data,
            (uint8_t)rxHeader.DLC
        );
    }
}

void canHandleMessage(
    uint32_t id,
    const uint8_t* data,
    uint8_t length
) {
    if (data == nullptr || length > 8) {
        return;
    }

    if (id == CAN_SET_ANGLE) {
        memcpy(lastAngleData, data, length);
        lastAngleLength = length;
    }
    else if (id == CAN_SET_SPEED) {
        memcpy(lastSpeedData, data, length);
        lastSpeedLength = length;
    }
    else if (id == CAN_ENCODER) {
        memcpy(lastEncoderData, data, length);
        lastEncoderLength = length;
    }
}
