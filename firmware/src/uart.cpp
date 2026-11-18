
#include "uart.h"

// USART2 handle
static UART_HandleTypeDef huart2;

// Initialize USART2
void uartInit(void) {

    // Enable peripheral clocks
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure UART TX and RX pins
    GPIO_InitTypeDef gpio = {};

    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &gpio);

    // Configure USART2
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    // Initialize UART
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        return;
    }
}

// Transmit a ControlPacket
HAL_StatusTypeDef uartSendPacket(const ControlPacket *packet) {

    // Check for a null pointer
    if (packet == nullptr) {
        return HAL_ERROR;
    }

    // Send the packet through USART2
    return HAL_UART_Transmit(
        &huart2,
        (uint8_t *)packet,
        sizeof(ControlPacket),
        10
    );
}
