#ifndef __RECIEVER_H__
#define __RECIEVER_H__

#include "stm32f3xx_hal.h"

/* This is the packet the ROS 2 node takes in per instance */
typedef struct __attribute__((packed)) {
    uint8_t header;     //0xAA tells    
    uint16_t steer;     // Channel 1 ~(1000 - 2000) 
    uint16_t throttle;  // Channel 4 ~(1000 - 2000) 
    uint16_t teleop;    // Channel 5 ~(1000 - 2000) 
    uint16_t deadman_switch;   // Channel 6 ~(1000 - 2000) 
} ControlPacket;

void recieverInit(void);
void recieverUpdatePacket(ControlPacket* p);
uint8_t recieverIsConnected(void);

#endif