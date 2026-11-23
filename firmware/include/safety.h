#pragma once

#include <stdint.h>
#include "recieverinput.h"

enum class SafetyState : uint8_t {
    DISABLED,
    READY,
    FAULT
};

SafetyState safetyEvaluate(
    const ControlPacket* packet,
    bool receiverConnected,
    bool communicationHealthy
);

bool safetyCanDrive(SafetyState state);