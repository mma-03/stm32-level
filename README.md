# STM32 Bare-Metal Digital Level

A two-axis digital level on an STM32F401RE (ARM Cortex-M4), written entirely at register level: no HAL, no vendor libraries, no code from configuration tools like STM32CubeMX. Startup code, linker script, and drivers written from the reference manual. Reads an MPU-6500 accelerometer over I2C, computes pitch and roll, smooths the readings, and lights an LED when the board is level. A button zeroes the reading on any surface.

## Hardware

- NUCLEO-F401RE
- GY-521 accelerometer module (sold as MPU6050, identified as MPU-6500)

| Sensor pin | Nucleo |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SCL | D15 (PB8) |
| SDA | D14 (PB9) |

## What's implemented

- **Startup and linker script:** hand-written vector table, `.data` copy and `.bss` zeroing, memory layout for 512 KB Flash / 96 KB SRAM, FPU enabled at reset.
- **Timer (TIM2):** interrupt-driven 1 ms tick. Fixed-rate main loop at 50 Hz.
- **UART (USART2):** 115200 baud debug output over the ST-LINK virtual COM port.
- **I2C (I2C1):** register-level controller driver written from the reference manual: start, address with ACK/NACK detection, write, single-byte read with NACK, stop, plus register read/write helpers.
- **MPU-6500 driver:** identity check, wake up, raw accelerometer read.
- **Tilt:** pitch and roll from the accelerometer with `atan2f`, a first-order low-pass filter (ALPHA = 0.1) on the raw readings, and button zeroing with edge detection.

## Notes from bring-up

**Wrong chip.** WHO_AM_I returned 0x70 instead of the MPU6050's 0x68, and PWR_MGMT_1 read 0x01 instead of 0x40. Both match the MPU-6500's documented power-up values, so the driver targets the MPU-6500 register map.

**Is it the desk or the sensor?** Flat on a desk, the X axis read about -670 instead of 0. That could come from the desk not being level, or from the sensor's own built-in error. Turning the board 180 degrees in place separates the two, because the desk's slope reverses when the board turns around but the sensor's error doesn't. So each reading is the error plus or minus the slope:

```
error + slope = -670 (before turning)
error - slope = +220 (after turning)
```

Adding the two readings cancels the slope, leaving the sensor's error: -225, or about 0.8 degrees. Subtracting them cancels the error, leaving the desk's slope: -445, or about 1.6 degrees. The zero button corrects for both.

**Filtering.** Raw readings varied by about +/- 100 counts at rest. Filtering the raw x/y/z values (rather than the angles) avoids errors where roll wraps from +180 degrees to -180 degrees.

## Build and flash

Requires `gcc-arm-none-eabi`.

```
make        # builds level.elf and level.bin
make flash  # copies level.bin to the Nucleo's ST-LINK drive
```

Output: serial terminal at 115200 baud.

## Project structure

```
include/ stm32f401.h timer.h uart.h i2c.h mpu6500.h tilt.h
src/ startup.c main.c timer.c uart.c i2c.c mpu6500.c tilt.c
linker.ld Makefile
```

## Known limitations

- Accelerometer values are read as six single-byte transfers, so the three axes aren't guaranteed to come from the same sample.
- The button is polled at 50 Hz rather than using an external interrupt.
- Accelerometer only. Adding the gyroscope with a complementary filter would give a steadier angle during movement.