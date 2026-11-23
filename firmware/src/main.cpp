
#include "stm32f3xx_hal.h"
#include "controller.h"

int main(void) {
    HAL_Init();

    controllerInit();

    while (1) {
        controllerUpdate();
        HAL_Delay(20);
    }
}
