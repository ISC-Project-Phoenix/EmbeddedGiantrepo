
#include "controller.h"
#include "config.h"
#include "recieverinput.h"
#include "safety.h"

static SafetyState currentState = SafetyState::DISABLED;

static uint16_t requestedSteering = PWM_NEUTRAL;
static uint16_t requestedThrottle = PWM_NEUTRAL;

void controllerInit(void) {
    currentState = SafetyState::DISABLED;
    requestedSteering = PWM_NEUTRAL;
    requestedThrottle = PWM_NEUTRAL;
}

void controllerProcessRC( const ControlPacket* packet, bool receiverConnected, bool communicationHealthy) {
    currentState = safetyEvaluate( packet, receiverConnected, communicationHealthy);

    if (currentState != SafetyState::READY) {
        requestedSteering = PWM_NEUTRAL;
        requestedThrottle = PWM_NEUTRAL;
        return;
    }

    if (packet != nullptr) {
        requestedSteering = packet->steer;
        requestedThrottle = packet->throttle;
    }
}

void controllerUpdate(void) {
    ControlPacket packet{};

    recieverUpdatePacket(&packet);

    bool receiverConnected = recieverIsConnected() != 0;
    
    bool communicationHealthy = receiverConnected;

    controllerProcessRC(
        &packet,
        receiverConnected,
        communicationHealthy
    );
}

SafetyState controllerGetState(void) {
    return currentState;
}

bool controllerCanDrive(void) {
    return safetyCanDrive(currentState);
}
