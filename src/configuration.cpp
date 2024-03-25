#include "configuration.h"
#include "data_def.h"
#include "utils.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <cstdint>
#include <string.h>

namespace cooboc {
Configuration::Configuration() {}
void Configuration::begin() {

  // Read EEPROM to memory
  const bool isPersistentHealthy = readFromEEPROM();
  if (!isPersistentHealthy) {
    Serial.println("first start, reset persistent.");
    persistent_.reset();
    writeToEEPROM();
  }

  // TODO: CHANGEME
  // strcpy(wifiSsid_, "astrolek");
  // strcpy(wifiPassword_, "rootroot");
  strcpy(wifiSsid_, "OnePlus 10 Pro 5G");
  strcpy(wifiPassword_, "rootroot");
}

bool Configuration::readFromEEPROM() {
  bool result = true;
  constexpr std::size_t PERSISTENT_LENGTH = sizeof(Persistent);
  EEPROM.begin(sizeof(PERSISTENT_LENGTH));
  std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
  for (std::size_t i{0U}; i < PERSISTENT_LENGTH; ++i) {
    *(targetAddr + 1) = EEPROM.read(i);
  }
  std::uint16_t readoutCrc = persistent_.crc;
  persistent_.crc = 0;
  std::uint16_t expectCrc = calculateCrc(targetAddr, PERSISTENT_LENGTH);
  return expectCrc == readoutCrc;
}

void Configuration::writeToEEPROM() {
  constexpr std::size_t PERSISTENT_LENGTH = sizeof(Persistent);
  std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
  persistent_.crc = 0U;
  std::uint16_t crc = calculateCrc(targetAddr, PERSISTENT_LENGTH);
  persistent_.crc = crc;
  for (std::size_t i{0U}; i < PERSISTENT_LENGTH; ++i) {
    EEPROM.write(i, targetAddr[i]);
  }
  EEPROM.commit();
}

} // namespace cooboc