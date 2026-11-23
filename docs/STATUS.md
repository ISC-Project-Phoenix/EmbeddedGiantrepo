# Implementation status

| Component | Current status |
| --- | --- |
| `main.cpp` | HAL init, PLL setup, receiver/controller init, loop |
| `recieverinput.cpp` | EXTI pulse capture, TIM1 counter, signal timeout; hardware validation pending |
| `safety.cpp` | RC packet validation and safety-state logic |
| `controller.cpp` | RC safety state and requested values, no actuator output |
| `pwm.cpp` | Start/stop/set compare implemented; `pwmInit` is a stub |
| `steering.cpp` | Stores requested angle; no physical output driver |
| `throttle.cpp` | Stores requested value; no physical output driver |
| `can.cpp` | Placeholder |
| `uart.cpp` | HAL UART functions exist, not integrated into main |
| Native tests | 11 Unity test cases supplied, not re-run in this packaging step |

## Hardware checks required

1. Verify NUCLEO-F303K8 clock tree and `TIM1` counting at 1 MHz.
2. Resolve pin conflicts between RC inputs and the original throttle/encoder design.
3. Check receiver channels with a logic analyzer or oscilloscope.
4. Confirm loss-of-signal and deadman behavior.
5. Design independent communication-health monitoring before ROS/CAN command handoff.
6. Bench-test outputs without motors/actuators connected.

**A successful compilation does not establish vehicle safety or functionality.**
