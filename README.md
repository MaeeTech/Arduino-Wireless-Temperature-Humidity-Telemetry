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

Receiver Unit

The receiver consists of an Arduino UNO, an nRF24L01 wireless transceiver, and an SH1106 OLED display.

Its main operations are:

Receiving wireless data packets.

Extracting the transmitted measurement values.

Recalculating and verifying the software CRC.

Checking packet sequence numbers.

Estimating the number of lost packets.

Converting the received integer values back into temperature and humidity measurements.

Displaying valid measurements on the OLED screen.

Detecting communication interruptions using a timeout mechanism.

Project Objectives
1) Temperature and environmental measurement
2) Wireless transmission of data from one board to another
3) Receiving and verifying the accuracy of the data
4) Detecting potential errors during data transmission
5) Counting lost packets and detecting connection loss
System Features

The main features of the implemented system include:

Wireless temperature and humidity monitoring.

2.4 GHz communication using nRF24L01 modules.

Separate transmitter and receiver programs.

Structured 8-byte data packet.

Software CRC-16/CCITT-FALSE error detection.

Hardware CRC support provided by the nRF24L01 modules.

Automatic acknowledgment (Auto ACK).

Automatic retransmission configuration.

Packet sequence number tracking.

Packet loss estimation.

Communication timeout detection.

Hardware Components

The following hardware components are used in this project:

Component

Quantity

Function

Arduino Pro Micro

1

Transmitter microcontroller

Arduino UNO

1

Receiver microcontroller

DHT11 Sensor

1

Temperature and humidity measurement

nRF24L01 Module

2

Wireless data transmission and reception

SH1106 OLED Display (128×64)

1

Displaying received measurements

Jumper Wires

As required

Electrical connections

Software Libraries
Library

Purpose

SPI.h

SPI communication with nRF24L01 modules

RF24.h

Configuration and control of nRF24L01 modules

DHT.h

Reading temperature and humidity from the DHT11 sensor

Wire.h

I2C communication with the OLED display

U8g2lib.h

OLED graphics and text rendering

stddef.h

Standard definitions, including structure offset calculations

stdint.h

Fixed-width integer data types


