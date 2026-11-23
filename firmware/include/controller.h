#pragma once

#include "recieverinput.h"
#include "safety.h"
#include <stdint.h>

// Initialize controller state
void controllerInit(void);

// Update controller logic
void controllerUpdate(void);

// Process receiver input
void controllerProcessRC(
    ControlPacket* packet,
    bool receiverConnected,
    bool communicationHealthy
);

// Get current safety state
SafetyState controllerGetState(void);

// Check if driving is permitted
bool controllerCanDrive(void);
