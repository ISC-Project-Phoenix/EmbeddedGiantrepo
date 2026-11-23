#ifndef RECIEVERINPUT_H
#define RECIEVERINPUT_H

#include <stdint.h>


// Packet sent to the ROS 2 node
typedef struct __attribute__((packed)) {
    uint8_t header;
    uint16_t steer;
    uint16_t throttle;
    uint16_t teleop;
    uint16_t deadman_switch;
} ControlPacket;

// Initialize receiver
void recieverInit(void);

// Update receiver packet
void recieverUpdatePacket(ControlPacket* p);

// Check receiver connection
uint8_t recieverIsConnected(void);

#endif
