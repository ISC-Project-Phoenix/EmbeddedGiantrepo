
#include "controller.h"
#include "recieverinput.h"
#include "safety.h"

static SafetyState currentState = SafetyState::DISABLED;

static uint16_t requestedSteering = 1500;
static uint16_t requestedThrottle = 1500;

void controllerInit(void) {
    currentState = SafetyState::DISABLED;
    requestedSteering = 1500;
    requestedThrottle = 1500;
}

void controllerProcessRC(
    const ControlPacket* packet,
    bool receiverConnected,
    bool communicationHealthy
) {
    currentState = safetyEvaluate(
        packet,
        receiverConnected,
        communicationHealthy
    );

    if (currentState != SafetyState::READY) {
        requestedSteering = 1500;
        requestedThrottle = 1500;
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
    bool communicationHealthy = false;

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
