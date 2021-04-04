# STM32 BluePill controlled by https://vcon.io

This is a baremetal firmware code for a STM32F103C8T6 "BluePill"
development board. This firmware blinks an LED, and lets user change blink
interval via UART - by entering numbers from 0 to 9.

This is a demonstration firmware for the VCON framework which implements
OTA firmware updates and remote control. See https://vcon.io/docs for details.

## Build

Install gcc-arm-none-eabi cross compiler on your system, then run:

```sh
make
```

That produces `bluepill.hex` file, suitable for remote firmware update
via VCON.
