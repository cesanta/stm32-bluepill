# STM32 BluePill controlled by the CCM

- Run `make`
- Connect using a 3.3V USB-to-Serial adapter: A9 to RXT, A10 to TXD, GND to GND, 3.3 to 3V3
- Set BOOT0 jumper to 1 then press RESET
- Run `make flash`
- Reset BOOT0 jumper to 0 then RESET again

See https://mongoose-os.com/docs/ccm/stm32-rest-api.md