# Wireless Temperature and Humidity Telemetry System

## 1. Project Overview

This project is an Arduino-based wireless telemetry system designed to measure and monitor environmental temperature and relative humidity.

The system uses a DHT11 sensor to collect environmental data and nRF24L01 modules to transmit the measurements wirelessly at 2.4 GHz. The received data is processed by an Arduino UNO and displayed on a 128×64 SH1106 OLED display.

## 2. Project Architecture
<img width="786" height="524" alt="image" src="https://github.com/user-attachments/assets/719ebe6c-989a-4252-8fbc-d70bce024f7a" />

The system consists of two main units:

### Transmitter Unit
- Arduino Pro Micro
- DHT11 Temperature and Humidity Sensor
- nRF24L01 Wireless Transceiver

The transmitter reads temperature and humidity measurements, processes the data, creates an 8-byte data packet, calculates its software CRC, and sends it wirelessly.

### Receiver Unit
- Arduino UNO
- nRF24L01 Wireless Transceiver
- SH1106 128×64 OLED Display

The receiver obtains the transmitted packets, checks their integrity, tracks packet sequence numbers, estimates packet losses, and displays valid measurements on the OLED screen.

## 3. Hardware Components

| Component | Quantity | Function |
|---|---:|---|
| Arduino Pro Micro | 1 | Transmitter controller |
| Arduino UNO | 1 | Receiver controller |
| DHT11 Sensor | 1 | Temperature and humidity measurement |
| nRF24L01 Module | 2 | Wireless communication |
| SH1106 OLED Display (128×64) | 1 | Displaying received data |
| Jumper Wires | As required | Electrical connections |

## 4. Pin Connections

### 4.1 Transmitter – Arduino Pro Micro

<img width="778" height="415" alt="image" src="https://github.com/user-attachments/assets/d2e66f18-87d6-4371-9d53-872ae9ab7290" />

| Module Pin | Arduino Pro Micro Pin |
|---|---|
| DHT11 DATA | D4 |
| nRF24L01 VCC | 3.3V |
| nRF24L01 GND | GND |
| nRF24L01 CE | D9 |
| nRF24L01 CSN | D10 |
| nRF24L01 SCK | D15 |
| nRF24L01 MOSI | D16 |
| nRF24L01 MISO | D14 |

### 4.2 Receiver – Arduino UNO

<img width="973" height="343" alt="image" src="https://github.com/user-attachments/assets/190772b6-7d88-4f4e-b924-f0f5f8adac29" />

| Module Pin | Arduino UNO Pin |
|---|---|
| nRF24L01 VCC | 3.3V |
| nRF24L01 GND | GND |
| nRF24L01 CE | D9 |
| nRF24L01 CSN | D10 |
| nRF24L01 SCK | D13 |
| nRF24L01 MOSI | D11 |
| nRF24L01 MISO | D12 |
| OLED SDA | A4 |
| OLED SCL | A5 |

**Note:** The nRF24L01 module requires a stable 3.3V power supply. The OLED power connections should follow the specifications of the particular display module.

### 4.3 Expected Output
<img width="719" height="404" alt="image" src="https://github.com/user-attachments/assets/2a4e7fe3-3a07-48e9-9211-e4988a2dc733" />


## 5. Software Libraries

The following libraries are used in the project:

| Library | Purpose |
|---|---|
| SPI.h | SPI communication |
| RF24.h | nRF24L01 configuration and wireless communication |
| DHT.h | Reading temperature and humidity |
| Wire.h | I2C communication with the OLED display |
| U8g2lib.h | OLED graphics and text rendering |
| stddef.h | Standard definitions and structure offset calculations |
| stdint.h | Fixed-width integer data types |

### External Libraries
- RF24 Library by TMRh20
- Adafruit DHT Sensor Library
- U8g2 Graphics Library by Oliver Kraus

## 6. Software and Development Tools

- **Arduino IDE:** Used for writing, compiling, uploading, and debugging the transmitter and receiver firmware.
- **Autodesk Tinkercad:** Used for circuit design and simulation where supported.
- **Arduino Serial Monitor:** Used for monitoring sensor readings, packet information, and communication status.

## 7. Main Technical Concepts

The project demonstrates several important embedded systems and wireless communication concepts:

- **Wireless Communication:** Data transmission using nRF24L01 modules in the 2.4 GHz band.
- **SPI Interface:** Communication between Arduino boards and nRF24L01 modules.
- **I2C Interface:** Communication between Arduino UNO and the OLED display.
- **Data Packet Structure:** Transmission of measurements and control information in an 8-byte packet.
- **Fixed-Point Representation:** Scaling temperature and humidity values by 100 before transmission.
- **CRC-16/CCITT-FALSE:** Software-based error detection for transmitted application data.
- **Hardware CRC:** Radio-level packet error detection using the nRF24L01.
- **Auto ACK:** Automatic acknowledgment and configured retransmission.
- **Sequence Number:** Identification of duplicate, outdated, and potentially lost packets.
- **Timeout Detection:** Identification of communication interruptions.

## 9. Project Applications

- Remote environmental monitoring
- Indoor temperature and humidity monitoring
- Greenhouse monitoring
- Storage environment monitoring
- Wireless sensor network experiments
- Educational embedded systems projects

## 10. Project Limitations

- Limited measurement accuracy of the DHT11 sensor.
- Wireless range depends on antenna performance, power supply, and environmental conditions.
- No permanent data logging is currently implemented.
- No Internet or cloud connectivity.
- No cryptographic encryption or authentication.
- Packet loss estimation is based on sequence number differences.
- The system is intended for educational and experimental applications, not safety-critical industrial monitoring.

## 11. Future Improvements

- Replacing DHT11 with a higher-accuracy sensor such as DHT22 or BME280.
- Adding SD card data logging.
- Developing a PC or web-based monitoring dashboard.
- Adding Wi-Fi or Internet connectivity.
- Improving wireless range and power management.
- Implementing data encryption and authentication.
- Evaluating communication range, packet delivery ratio, and measurement accuracy through practical tests.

## 12. Author

**Name:** [MaeeTech]  
**Field:** Electrical Engineering  
**Project Title:** Wireless Temperature and Humidity Telemetry System  
**Platform:** Arduino-Based Embedded System

