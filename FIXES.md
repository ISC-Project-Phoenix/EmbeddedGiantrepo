# Fixes in this version

## Firmware bugs fixed
1. `main.cpp` never called `recieverInit()`, so the receiver (pins, timer,
   interrupts) never started. It is now called after `HAL_Init()`.
2. `controller.cpp` hard-coded `communicationHealthy = false`, so the safety
   state was always FAULT and the kart could never drive. It now uses the
   receiver connection as the health signal (replace with a real heartbeat later).
3. No clock setup: the chip stayed on the 8 MHz default clock while the receiver
   timer assumes 64 MHz (prescaler 63 -> 1 us ticks). `main.cpp` now configures
   HSI -> PLL x16 = 64 MHz (APB1 32 MHz, APB2 64 MHz).
4. No SysTick handler: `SysTick_Handler` was only the weak default, so the HAL
   tick never advanced and `HAL_Delay(20)` would hang. `main.cpp` now defines it.
5. `pwmStart` and `pwmSetCompare` always returned `HAL_ERROR`. They now call
   `HAL_TIM_PWM_Start` and `__HAL_TIM_SET_COMPARE`.

## Cleanup
- `config.h` was a copy of `can.h`. It now holds the shared PWM / timeout
  constants, and `safety.cpp`, `recieverinput.cpp`, `controller.cpp` use them
  instead of repeating the numbers.
- `.gitignore` now ignores `firmware/compile_commands.json` (it had C:\Users\... paths).
- `.gitattributes` (`* text=auto`) stops CRLF/LF from showing every file as modified.
- The nested `EmbeddedGiantrepotest/` repo is not included here (see git steps below).
- `platformio.ini`: added an `[env:native]` so `pio test -e native` runs on your PC;
  the board env ignores tests.
- `test_safety.cpp` rewritten with Unity: 11 tests, including controller
  regression tests that fail on the old controller.

## NOT done (still placeholders)
- Steering / throttle outputs: `pwmInit`, `steeringInit`, `throttleInit` still
  return `HAL_ERROR`; the controller does not drive any output yet. This needs
  your real timer/pin choices.
- CAN (`can.cpp`) and `uartInit()` / `uartSendPacket()` are still unimplemented / unused.

## How to apply
1. Unzip, then copy everything inside `EmbeddedGiantrepo/` over your repo folder
   (overwrite when asked).
2. In your repo, remove the nested repo and the machine-specific file from git:
       git rm --cached EmbeddedGiantrepotest
       git rm --cached firmware/compile_commands.json
   Then move the inner `EmbeddedGiantrepotest` folder out of the repo
   (it is its own git repo with its own history).
3. `git status` should now show only the files listed above as changed.
4. Run `pio test -e native`, then `pio run -e nucleo_f303k8`, and test on the board.

## Verified here vs not verified
- Verified: all 11 tests pass on a PC (real Unity library); the 2 controller
  regression tests fail on the old controller. `main.cpp`, `pwm.cpp` and
  `recieverinput.cpp` pass a syntax check against a mock HAL header.
- NOT verified: a real STM32 build, flashing, or the clock/receiver timing on
  hardware. Check the 1000-2000 us pulse readings on the board.
