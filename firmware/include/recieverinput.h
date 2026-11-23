#ifndef RECIEVERINPUT_H
#define RECIEVERINPUT_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t header;
    uint16_t steer;
    uint16_t throttle;
    uint16_t teleop;
    uint16_t deadman_switch;
} ControlPacket;

void recieverInit(void);
void recieverUpdatePacket(ControlPacket* p);
uint8_t recieverIsConnected(void);

#endif