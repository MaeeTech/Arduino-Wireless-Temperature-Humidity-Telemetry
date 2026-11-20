#include <SPI.h>
#include <RF24.h>
#include <DHT.h>
#include <stddef.h>
#include <stdint.h>

// -------- PIN SETTINGS --------
#define DHT_PIN 4
#define DHT_TYPE DHT11

#define CE_PIN  9
#define CSN_PIN 10

// -------- OBJECTS --------
RF24 radio(CE_PIN, CSN_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

const byte address[6] = "TEL01";

// -------- PACKET STRUCTURE --------
struct Packet {
  uint16_t sequence;
  int16_t temperature_x100;
  uint16_t humidity_x100;
  uint16_t crc;
};

uint16_t sequenceNumber = 0;

// -------- CRC16 FUNCTION --------
// CRC-16/CCITT-FALSE

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

// -------- SETUP --------

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!radio.begin()) {
    Serial.println(F("ERROR: Radio not detected!"));
    while (1) {}
  }
radio.setPayloadSize(sizeof(Packet));
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  // Hardware CRC
  radio.setCRCLength(RF24_CRC_16);

  // Automatic acknowledgment and retry
  radio.setAutoAck(true);
  radio.setRetries(5, 15);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println(F("TELEMETRY TRANSMITTER READY"));
}

// -------- MAIN LOOP --------

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("DHT SENSOR ERROR"));
    delay(1000);
    return;
  }

  Packet packet = {};

  packet.sequence = sequenceNumber++;

  packet.temperature_x100 =
      (int16_t)(temperature * 100.0f);

  packet.humidity_x100 =
      (uint16_t)(humidity * 100.0f);

  // Calculate software CRC
  packet.crc = calculateCRC(
      (const uint8_t *)&packet,
      offsetof(Packet, crc)
  );

  // Send packet
  bool ack = radio.write(&packet, sizeof(packet));

  Serial.print(F("Sequence: "));
  Serial.println(packet.sequence);

  Serial.print(F("Temperature: "));
  Serial.print(temperature, 1);
  Serial.println(F(" C"));

  Serial.print(F("Humidity: "));
  Serial.print(humidity, 1);
  Serial.println(F(" %"));

  Serial.print(F("Software CRC: 0x"));
  Serial.println(packet.crc, HEX);

  Serial.print(F("Radio ACK: "));
  Serial.println(ack ? F("SUCCESS") : F("FAILED"));

  Serial.println(F("----------------------"));

  delay(1000);
}
