#include <RadioHead.h>

#include <RH_ASK.h>
#include <SPI.h>

#define RX_PIN  11

/// \param[in] speed The desired bit rate in bits per second
/// \param[in] rxPin The pin that is used to get data from the receiver
/// \param[in] txPin The pin that is used to send data to the transmitter
/// \param[in] pttPin The pin that is connected to the transmitter controller. It will be set HIGH to enable the transmitter (unless pttInverted is true).
/// \param[in] pttInverted true if you desire the pttin to be inverted so that LOW wil enable the transmitter.
//RH_ASK driver(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);

RH_ASK driver(2000, RX_PIN, 12, 10, false);

void setup() {
  Serial.begin(9600); // Debugging only
  Serial.println("Start RFReceiverDemo");
  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf) - 1;

  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0';
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    Serial.println((char*)buf);
    Serial.println(buflen);
  }
}
