// a simulation test for safety 

#include <cassert>
#include "safety.h"

int main() {
    ControlPacket packet{};

    packet.header = 0xAA;
    packet.steer = 1500;
    packet.throttle = 1500;
    packet.teleop = 2000;
    packet.deadman_switch = 2000;

    assert(safetyEvaluate(&packet, true, true)
           == SafetyState::READY);

    packet.deadman_switch = 1000;

    assert(safetyEvaluate(&packet, true, true)
           == SafetyState::DISABLED);

    packet.deadman_switch = 2000;

    assert(safetyEvaluate(&packet, false, true)
           == SafetyState::FAULT);

    packet.steer = 2500;

    assert(safetyEvaluate(&packet, true, true)
           == SafetyState::FAULT);
}