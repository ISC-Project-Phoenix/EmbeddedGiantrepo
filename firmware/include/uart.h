#ifndef UART_H
#define UART_H

#include "stm32f3xx_hal.h"
#include "recieverinput.h"
#include <stdint.h>

// Initialize USART2
void uartInit(void);

// Transmit a ControlPacket through UART
HAL_StatusTypeDef uartSendPacket(
    const ControlPacket *packet
);
#endif 