#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <stddef.h>
#include <stdint.h>

// -------- OLED SETTINGS --------

// SH1106 128x64 I2C
// Page buffer saves SRAM on Arduino Uno
U8G2_SH1106_128X64_NONAME_1_HW_I2C
u8g2(U8G2_R0, U8X8_PIN_NONE);

// -------- RADIO SETTINGS --------
#define CE_PIN  9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "TEL01";

// -------- PACKET STRUCTURE --------
struct Packet {
  uint16_t sequence;
  int16_t temperature_x100;
  uint16_t humidity_x100;
  uint16_t crc;
};

// -------- VARIABLES --------
bool haveSequence = false;
uint16_t lastSequence = 0;

uint32_t lostPackets = 0;
uint32_t lastValidPacketTime = 0;

bool linkTimedOut = true;

float temperature = 0;
float humidity = 0;

// -------- CRC16 FUNCTION --------

uint16_t calculateCRC(const uint8_t *data, size_t length) {
  uint16_t crc = 0xFFFF;

  for (size_t i = 0; i < length; i++) {
    crc ^= (uint16_t)data[i] << 8;

    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}

// -------- OLED WAITING SCREEN --------

void showWaiting() {
  u8g2.firstPage();

  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(12, 25, "TELEMETRY");

    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(15, 48, "Waiting for data...");
  } while (u8g2.nextPage());
}

// -------- OLED DATA SCREEN --------

void showData(uint16_t sequence) {
  u8g2.firstPage();

  do {
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 10, "WIRELESS TELEMETRY");
    u8g2.drawHLine(0, 14, 128);

    u8g2.setFont(u8g2_font_ncenB14_tr);

    u8g2.setCursor(0, 34);
    u8g2.print("T:");
    u8g2.print(temperature, 1);
    u8g2.print(" C");

    u8g2.setCursor(0, 53);
    u8g2.print("H:");
    u8g2.print(humidity, 1);
    u8g2.print(" %");

    u8g2.setFont(u8g2_font_5x8_tr);

    u8g2.setCursor(0, 63);
    u8g2.print("S:");
    u8g2.print(sequence);

    u8g2.setCursor(65, 63);
    u8g2.print("Lost:");
    u8g2.print(lostPackets);

  } while (u8g2.nextPage());
}

// -------- OLED NO SIGNAL SCREEN --------

void showNoSignal() {
  u8g2.firstPage();

  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(5, 27, "NO SIGNAL");

    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(12, 48, "Check transmitter");

  } while (u8g2.nextPage());
}

// -------- SETUP --------

void setup() {
  Serial.begin(115200);

  // Start OLED
  u8g2.begin();
  showWaiting();

  // Start radio
  if (!radio.begin()) {
    u8g2.firstPage();

    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(10, 30, "RADIO ERROR");
    } while (u8g2.nextPage());

    Serial.println(F("RADIO ERROR"));
    while (1) {}
  }
radio.setPayloadSize(sizeof(Packet));
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);

  radio.openReadingPipe(1, address);
  radio.startListening();

  Serial.println(F("TELEMETRY RECEIVER READY"));
}

// -------- MAIN LOOP --------

void loop() {

  if (radio.available()) {

    Packet packet = {};

    radio.read(&packet, sizeof(packet));

    // Check software CRC
    uint16_t expectedCRC = calculateCRC(
        (const uint8_t *)&packet,
        offsetof(Packet, crc)
    );

    if (packet.crc != expectedCRC) {
      Serial.println(F("SOFTWARE CRC ERROR"));
    } else {

      // Check sequence number
      bool validSequence = true;

      if (haveSequence) {
        uint16_t difference =
            (uint16_t)(packet.sequence - lastSequence);

        // Reject duplicate or old packets
        if (difference == 0 || difference >= 32768) {
          validSequence = false;
          Serial.println(F("DUPLICATE / OLD PACKET"));
        } else if (difference > 1) {
          lostPackets += (uint16_t)(difference - 1);
        }
      }

      if (validSequence) {

        // Check data range
        float newTemperature =
            packet.temperature_x100 / 100.0f;

        float newHumidity =
            packet.humidity_x100 / 100.0f;

        if (newHumidity <= 100.0f &&
            newTemperature >= -40.0f &&
            newTemperature <= 125.0f) {

          lastSequence = packet.sequence;
          haveSequence = true;

          temperature = newTemperature;
          humidity = newHumidity;

          lastValidPacketTime = millis();
          linkTimedOut = false;

          showData(packet.sequence);

          Serial.print(F("VALID PACKET: "));
          Serial.println(packet.sequence);

          Serial.print(F("Temperature: "));
          Serial.println(temperature, 2);

          Serial.print(F("Humidity: "));
          Serial.println(humidity, 2);

          Serial.print(F("Lost packets: "));
          Serial.println(lostPackets);

          Serial.println(F("----------------"));
        } else {
          Serial.println(F("INVALID SENSOR DATA"));
        }
      }
    }
  }

  // Link timeout after 3 seconds
  if (!linkTimedOut &&
      (uint32_t)(millis() - lastValidPacketTime) > 3000UL) {

    linkTimedOut = true;

    showNoSignal();

    Serial.println(F("LINK TIMEOUT"));
  }
}
