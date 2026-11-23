#include "controller.h"
#include "recieverinput.h"
#include "safety.h"

static SafetyState currentState = SafetyState::DISABLED;

void controllerInit(void) {
    currentState = SafetyState::DISABLED;
}

void controllerProcessRC(
    const ControlPacket* packet,
    bool receiverConnected,
    bool communicationHealthy
) {
    // Evaluate safety conditions
    currentState = safetyEvaluate(
        packet,
        receiverConnected,
        communicationHealthy
    );

    // TODO: Handle FAULT state

    // TODO: Handle DISABLED state

    // TODO: Handle READY state
    // Only prepare commands here for now.
    // Do not activate throttle or steering yet.
}

void controllerUpdate(void) {
    // TODO: Read the latest receiver packet

    // TODO: Check receiver signal freshness

    // TODO: Check communication timeout

    // TODO: Call controllerProcessRC()

    // TODO: Update steering and throttle
    // only after safety checks are implemented
}

SafetyState controllerGetState(void) {
    return currentState;
}

bool controllerCanDrive(void) {
    return safetyCanDrive(currentState);
}