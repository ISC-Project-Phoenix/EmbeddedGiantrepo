#include "recieverinput.h"

#define CHANNEL_COUNT 4

#define STEERING 0
#define THROTTLE 1
#define TELEOP   2
#define DEADMAN  3

#define SIGNAL_TIMEOUT 100   // ms without a valid pulse = channel dead
#define NEUTRAL_PWM    1500
#define FAILPWM        0     // impossible real pulse, so it can only mean failsafe
#define DEADMAN_MIN    1700  // deadman must be above this to allow driving

static TIM_HandleTypeDef timer;

static volatile uint16_t pulseStart[CHANNEL_COUNT] = {0};
static volatile uint8_t  pulseStarted[CHANNEL_COUNT] = {0};
static volatile uint16_t pulseWidth[CHANNEL_COUNT] = {1500, 1500, 1500, 1500};
static volatile uint32_t lastUpdate[CHANNEL_COUNT] = {0};
static volatile uint8_t  receiverReady = 0;


void recieverInit(void) {
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOA, &gpio);

    // Assumes TIM1 input clock = 64 MHz, so prescaler 63 gives a 1 MHz (1 us) tick.
    // If the clock tree changes, this value must change too.
    timer.Instance = TIM1;
    timer.Init.Prescaler = 63;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Init.Period = 0xFFFF;
    timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_Base_Init(&timer) != HAL_OK) return;
    if (HAL_TIM_Base_Start(&timer) != HAL_OK) return;

    IRQn_Type interrupts[] = { EXTI0_IRQn, EXTI1_IRQn, EXTI3_IRQn, EXTI4_IRQn };

    for (IRQn_Type irq : interrupts) {
        HAL_NVIC_SetPriority(irq, 1, 0);
        HAL_NVIC_EnableIRQ(irq);
    }

    receiverReady = 1;   // only reached if everything above succeeded
}


// Single place that decides whether all channels are alive
static uint8_t channelsAlive(const volatile uint32_t *ts, uint32_t now) {
    if (!receiverReady) return 0;

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        if (ts[i] == 0 || (now - ts[i]) > SIGNAL_TIMEOUT) {
            return 0;
        }
    }
    return 1;
}


// Check whether all receiver channels are updating
uint8_t recieverIsConnected(void) {
    return channelsAlive(lastUpdate, HAL_GetTick());
}


// Update the control packet
void recieverUpdatePacket(ControlPacket *packet) {
    uint16_t values[CHANNEL_COUNT];
    uint32_t timestamps[CHANNEL_COUNT];

    uint32_t irqState = __get_PRIMASK();
    __disable_irq();

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        values[i] = pulseWidth[i];
        timestamps[i] = lastUpdate[i];
    }

    __set_PRIMASK(irqState);

    uint32_t now = HAL_GetTick();
    uint8_t connected = channelsAlive(timestamps, now);

    packet->header = 0xAA;

    if (!connected || values[DEADMAN] < DEADMAN_MIN) {
        packet->steer = NEUTRAL_PWM;
        packet->throttle = NEUTRAL_PWM;
        packet->teleop = FAILPWM;
        packet->deadman_switch = 1000;
    }
    else {
        packet->steer = values[STEERING];
        packet->throttle = values[THROTTLE];
        packet->teleop = values[TELEOP];
        packet->deadman_switch = values[DEADMAN];
    }
}


// Handle incoming PWM signal edges
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    int channel = -1;

    if (pin == GPIO_PIN_0) channel = STEERING;
    else if (pin == GPIO_PIN_1) channel = THROTTLE;
    else if (pin == GPIO_PIN_3) channel = TELEOP;
    else if (pin == GPIO_PIN_4) channel = DEADMAN;
    else return;

    uint16_t currentTime = (uint16_t)__HAL_TIM_GET_COUNTER(&timer);

    if (HAL_GPIO_ReadPin(GPIOA, pin) == GPIO_PIN_SET) {
        pulseStart[channel] = currentTime;
        pulseStarted[channel] = 1;
    }
    else if (pulseStarted[channel]) {
        pulseStarted[channel] = 0;

        uint16_t width = (uint16_t)(currentTime - pulseStart[channel]);

        // Accept a slightly wider range so full-stick pulses still count as alive,
        // then clamp to the normal 1000-2000 range.
        if (width >= 900 && width <= 2100) {
            if (width < 1000) width = 1000;
            if (width > 2000) width = 2000;

            pulseWidth[channel] = width;
            lastUpdate[channel] = HAL_GetTick();
        }
    }
}