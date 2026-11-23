#include "stm32f3xx_hal.h"
#include "controller.h"
#include "recieverinput.h"
#include "can.h"

static void errorHalt(void) {
    __disable_irq();

    while (1) {
    }
}

static void systemClockConfig(void) {
    RCC_OscInitTypeDef osc = {};
    RCC_ClkInitTypeDef clk = {};

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState = RCC_HSI_ON;
    osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    osc.PLL.PLLMUL = RCC_PLL_MUL16;

    if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
        errorHalt();
    }

    clk.ClockType = RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 |
                    RCC_CLOCKTYPE_PCLK2;

    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV2;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2) != HAL_OK) {
        errorHalt();
    }
}

extern "C" void SysTick_Handler(void) {
    HAL_IncTick();
}

int main(void) {
    HAL_Init();

    systemClockConfig();

    recieverInit();
    controllerInit();

    if (canInit() != HAL_OK) {
        errorHalt();
    }

    while (1) {
        controllerUpdate();
        canProcessReceived();

        HAL_Delay(20);
    }
}
