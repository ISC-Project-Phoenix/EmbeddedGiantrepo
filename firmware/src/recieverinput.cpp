
#include "recieverinput.h"

// Receiver channels
#define CHANNEL_COUNT 4

#define STEERING  0
#define THROTTLE  1
#define TELEOP    2
#define DEADMAN   3

#define SIGNAL_TIMEOUT 100
#define NEUTRAL_PWM 1500

static TIM_HandleTypeDef timer;

// PWM measurements
static volatile uint16_t pulseStart[CHANNEL_COUNT] = {0};

static volatile uint16_t pulseWidth[CHANNEL_COUNT] = {
    1500, 1500, 1500, 1500
};

static volatile uint32_t lastUpdate[CHANNEL_COUNT] = {0};


// Initialize receiver GPIO pins
void recieverInit(void) {
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 |
               GPIO_PIN_3 | GPIO_PIN_4;

    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOA, &gpio);

    timer.Instance = TIM1;
    timer.Init.Prescaler = 63;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Init.Period = 0xFFFF;

    HAL_TIM_Base_Init(&timer);
    HAL_TIM_Base_Start(&timer);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}


// Check receiver connection
uint8_t recieverIsConnected(void) {
    uint32_t now = HAL_GetTick();

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        if (lastUpdate[i] == 0 ||
            now - lastUpdate[i] > SIGNAL_TIMEOUT) {
            return 0;
        }
    }

    return 1;
}


// Copy receiver values into packet
void recieverUpdatePacket(ControlPacket *packet) {
    uint16_t values[CHANNEL_COUNT];

    __disable_irq();

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        values[i] = pulseWidth[i];
    }

    __enable_irq();

    packet->header = 0xAA;
    packet->deadman_switch = values[DEADMAN];

    if (!recieverIsConnected() ||
        values[DEADMAN] < 1500) {

        packet->steer = NEUTRAL_PWM;
        packet->throttle = NEUTRAL_PWM;
        packet->teleop = NEUTRAL_PWM;
    }
    else {
        packet->steer = values[STEERING];
        packet->throttle = values[THROTTLE];
        packet->teleop = values[TELEOP];
    }
}


// Handle incoming PWM signals
void HAL_GPIO_EXTI_Callback(uint16_t pin) {

    // Check valid receiver pins
    if (pin == GPIO_PIN_0 ||  pin == GPIO_PIN_1 ||  pin == GPIO_PIN_3 || pin == GPIO_PIN_4) {

        
    }
    else {
        return;
    }

    int channel = -1;

    if (pin == GPIO_PIN_0) channel = STEERING;
    else if (pin == GPIO_PIN_1) channel = THROTTLE;
    else if (pin == GPIO_PIN_3) channel = TELEOP;
    else if (pin == GPIO_PIN_4) channel = DEADMAN;

    uint16_t currentTime =
        (uint16_t)__HAL_TIM_GET_COUNTER(&timer);

    if (HAL_GPIO_ReadPin(GPIOA, pin) == GPIO_PIN_SET) {
        pulseStart[channel] = currentTime;
    }
    else {
        pulseWidth[channel] =
            (uint16_t)(currentTime - pulseStart[channel]);

        lastUpdate[channel] = HAL_GetTick();
    }
}
