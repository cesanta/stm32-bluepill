# STM32 BluePill controlled by the CCM

## Bluepill with CCM: remote OTA and RESTful service

- Connect Bluepill to CCM: 3.3V-3.3v, GND-GND, PA9-IO25, PA10-IO26, BOOT0-IO27, RST-IO14
- Connect CCM to your workstation over the USB
- Login to https://dash.mongoose-os.com, create new device
- `mos flash ccm`, `mos wifi NET PASS`

## Stand-alone: Bluepill is a JSON-RPC server over UART

- Run `make`
- Connect using a 3.3V USB-to-Serial adapter: A9 to RXT, A10 to TXD, GND to GND, 3.3 to 3V3
- Set BOOT0 jumper to 1 then press RESET
- Run `make flash`
- Reset BOOT0 jumper to 0 then RESET again

See https://mongoose-os.com/docs/ccm/stm32-rest-api.md
