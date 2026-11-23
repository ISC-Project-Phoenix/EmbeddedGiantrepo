// Host-side tests for the safety logic and controller.
// Run with: pio test -e native

#include <unity.h>
#include "safety.h"
#include "controller.h"

// ---- Fakes for the hardware receiver (controller.cpp calls these) ----
static ControlPacket fakePacket;
static uint8_t fakeConnected = 0;

void recieverUpdatePacket(ControlPacket* p) {
    *p = fakePacket;
}

uint8_t recieverIsConnected(void) {
    return fakeConnected;
}

static ControlPacket goodPacket() {
    ControlPacket p{};
    p.header = 0xAA;
    p.steer = 1500;
    p.throttle = 1500;
    p.teleop = 2000;
    p.deadman_switch = 2000;
    return p;
}

void setUp(void) {
    fakePacket = goodPacket();
    fakeConnected = 1;
    controllerInit();
}

void tearDown(void) {
}

#define ASSERT_STATE(expected, actual) \
    TEST_ASSERT_EQUAL_INT((int)(expected), (int)(actual))

// ---- safetyEvaluate ----

void test_ready_when_everything_ok(void) {
    ControlPacket p = goodPacket();
    ASSERT_STATE(SafetyState::READY, safetyEvaluate(&p, true, true));
}

void test_deadman_released_is_disabled(void) {
    ControlPacket p = goodPacket();
    p.deadman_switch = 1000;
    ASSERT_STATE(SafetyState::DISABLED, safetyEvaluate(&p, true, true));
}

void test_teleop_off_is_disabled(void) {
    ControlPacket p = goodPacket();
    p.teleop = 1000;
    ASSERT_STATE(SafetyState::DISABLED, safetyEvaluate(&p, true, true));
}

void test_receiver_disconnected_is_fault(void) {
    ControlPacket p = goodPacket();
    ASSERT_STATE(SafetyState::FAULT, safetyEvaluate(&p, false, true));
}

void test_comms_unhealthy_is_fault(void) {
    ControlPacket p = goodPacket();
    ASSERT_STATE(SafetyState::FAULT, safetyEvaluate(&p, true, false));
}

void test_out_of_range_pwm_is_fault(void) {
    ControlPacket p = goodPacket();
    p.steer = 2500;
    ASSERT_STATE(SafetyState::FAULT, safetyEvaluate(&p, true, true));
}

void test_bad_header_is_fault(void) {
    ControlPacket p = goodPacket();
    p.header = 0x55;
    ASSERT_STATE(SafetyState::FAULT, safetyEvaluate(&p, true, true));
}

void test_null_packet_is_fault(void) {
    ASSERT_STATE(SafetyState::FAULT, safetyEvaluate(nullptr, true, true));
}

// ---- controller (regression: communicationHealthy used to be hard-coded false) ----

void test_controller_can_drive_with_good_receiver(void) {
    controllerUpdate();
    ASSERT_STATE(SafetyState::READY, controllerGetState());
    TEST_ASSERT_TRUE(controllerCanDrive());
}

void test_controller_blocks_when_receiver_lost(void) {
    fakeConnected = 0;
    controllerUpdate();
    ASSERT_STATE(SafetyState::FAULT, controllerGetState());
    TEST_ASSERT_FALSE(controllerCanDrive());
}

void test_controller_blocks_when_deadman_released(void) {
    fakePacket.deadman_switch = 1000;
    controllerUpdate();
    ASSERT_STATE(SafetyState::DISABLED, controllerGetState());
    TEST_ASSERT_FALSE(controllerCanDrive());
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    UNITY_BEGIN();
    RUN_TEST(test_ready_when_everything_ok);
    RUN_TEST(test_deadman_released_is_disabled);
    RUN_TEST(test_teleop_off_is_disabled);
    RUN_TEST(test_receiver_disconnected_is_fault);
    RUN_TEST(test_comms_unhealthy_is_fault);
    RUN_TEST(test_out_of_range_pwm_is_fault);
    RUN_TEST(test_bad_header_is_fault);
    RUN_TEST(test_null_packet_is_fault);
    RUN_TEST(test_controller_can_drive_with_good_receiver);
    RUN_TEST(test_controller_blocks_when_receiver_lost);
    RUN_TEST(test_controller_blocks_when_deadman_released);
    return UNITY_END();
}
