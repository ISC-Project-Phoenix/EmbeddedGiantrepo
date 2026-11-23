#include "safety.h"
#include "config.h"


static bool validPWM(uint16_t value) {
    return value >= PWM_MIN && value <= PWM_MAX;
}


SafetyState safetyEvaluate(
    const ControlPacket* packet,
    bool receiverConnected,
    bool communicationHealthy
) {

   
    if (packet == nullptr) {
        return SafetyState::FAULT;
    }

    
    if (!receiverConnected || !communicationHealthy) {
        return SafetyState::FAULT;
    }

  
    if (packet->header != 0xAA) {
        return SafetyState::FAULT;
    }


    if (!validPWM(packet->steer) ||
        !validPWM(packet->throttle) ||
        !validPWM(packet->teleop) ||
        !validPWM(packet->deadman_switch)) {

        return SafetyState::FAULT;
    }

    
    if (packet->deadman_switch < DEADMAN_MIN) {
        return SafetyState::DISABLED;
    }


    if (packet->teleop < TELEOP_MIN) {
        return SafetyState::DISABLED;
    }

    return SafetyState::READY;
}


bool safetyCanDrive(SafetyState state) {
    return state == SafetyState::READY;
}
