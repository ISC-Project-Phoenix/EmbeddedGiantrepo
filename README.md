# Project Phoenix — STM32 Firmware

This repository contains the STM32 firmware, host-side safety tests, and placeholders for ROS 2 integration.

## Folder structure

- `firmware/src/` — firmware implementations and MCU entry point
- `firmware/include/` — shared headers and configuration
- `firmware/test/test_safety/` — Unity safety/controller tests
- `firmware/lib/` — space for reusable drivers and utilities
- `ros2/` — ROS 2 integration placeholder (not implemented)
- `docs/` — project documentation
- `scripts/` — project scripts placeholder

## Build (Windows CMD)

Open a terminal inside `firmware`:

```cmd
C:\Users\saadi\.platformio\penv\Scripts\pio.exe run -e nucleo_f303k8
```

## Test (Windows CMD)

```cmd
C:\Users\saadi\.platformio\penv\Scripts\pio.exe test -e native
```

If `pio` is on your PATH, you can use `pio run -e nucleo_f303k8` and `pio test -e native` instead.

## Current limitations

- The receiver initialization and clock setup have been added, but board-level timing and pin assignments are **not verified**.
- CAN, steering output, throttle output, and full ROS 2 communication are **not implemented**.
- The controller does not actuate steering or throttle.
- `communicationHealthy` currently mirrors receiver connectivity; it is **not** a separate ROS/CAN heartbeat.
- Do not connect motor or steering actuators until output behavior, deadman, watchdog, E-stop, and electrical pin mapping have been validated.

See `docs/STATUS.md` and `docs/GITHUB_UPLOAD.md`.
