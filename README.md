# STM32 BluePill controlled by Cloud Connector

This is a baremetal firmware code for a STM32F103C8T6 "BluePill"
development board. This firmware blinks an LED, and runs a JSON-RPC
client/server over the UART. With this setup, this board can be wired to the
https://vcon.io, which gives the following functionality:

- Allows to remotely flash the BluePill board
- Allows to call BluePill's functions over the network
- Allows BluePill to call VCON functions like MQTT publish/subcribe, create/edit/delete/download files, etc

Basically, VCON grants remote control capabilities without
writing a single line of networking code, and grants Over-the-Air firmware
update without writing a single line of code.

Follow https://vcon.io for detailed instrustions.

