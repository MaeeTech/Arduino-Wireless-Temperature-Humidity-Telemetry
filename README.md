# Arduino-Wireless-Temperature-Humidity-Telemetry
Wireless temperature and humidity monitoring and telemetry system using Arduino Pro Micro, Arduino UNO, DHT11 sensor, and nRF24L01 wireless communication modules.

System Overview

This project is a wireless temperature and humidity telemetry system designed to measure environmental conditions and transmit the collected data between two Arduino-based units.

The system consists of a transmitter and a receiver.

Transmitter Unit
The transmitter consists of an Arduino Pro Micro, a DHT11 sensor, and an nRF24L01 wireless transceiver.
Its main operations are:
▪Measuring temperature and relative humidity using the DHT11 sensor.
▪Reading and processing the sensor measurements.
▪Converting floating-point measurements into scaled integer values.
▪Assigning a sequence number to each data packet.
▪Calculating the software CRC value.
▪Transmitting the prepared data packet through the nRF24L01 module.
