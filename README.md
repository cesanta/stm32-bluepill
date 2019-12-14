# STM32 BluePill controlled by Cloud Connector

This is a baremetal firmware code for a STM32F103C8T6 "BluePill"
development board. This firmware blinks an LED, and runs a JSON-RPC
instance over the UART. With this setup, this board can be wired to the
[ESP32 Cloud Connector](https://mdash.net/docs/quickstart/ccm.md), which
gives the following functionality:

- Allows to call BluePill's functions over the network
- Allows BluePill to call Cloud Connector's functions like
  MQTT publish/subcribe, create/edit/delete/download files, etc
- Allows to remotely flash the BluePill board

Basically, a Cloud Connector grants remote control capabilities without
writing a single line of networking code, and grants Over-the-Air firmware
update without writing a single line of code.

Follow
[ESP32 Cloud Connector BluePill Guide](https://mdash.net/docs/quickstart/ccm.md#example-ota-for-stm32f103c8t6-bluepill)
for detailed instrustions.

