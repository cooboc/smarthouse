#include "configuration.h"
#include "data_def.h"
#include "test_gear.h"
#include "utils.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <cstdint>
#include <string.h>

namespace cooboc {
namespace {

static constexpr char CHARMAP[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void makePrintableId(std::uint32_t id, char buf[7]) {
  constexpr std::size_t CHARMAP_LENGTH = sizeof(CHARMAP) - 1U;
  for (std::size_t i{0U}; i < 6; ++i) {
    buf[5U - i] = id % CHARMAP_LENGTH;
    id /= CHARMAP_LENGTH;
  }

  if (id > 0) {
    buf[0] += id;
    buf[0] %= CHARMAP_LENGTH;
  }
  for (std::size_t i{0U}; i < 6U; ++i) {
    buf[i] = CHARMAP[buf[i]];
  }
  buf[6] = '\0';
}

} // namespace

Configuration::Configuration() : gearList_{new TestGear()} {}
void Configuration::begin() {
  debugPrintGears();
  const std::uint32_t chipId = ESP.getChipId();
  constexpr char NAMEPREFIX[] = "csg-";
  constexpr std::size_t NAMEPREFIX_LENGTH = sizeof(NAMEPREFIX) - 1U;
  makePrintableId(chipId, idStr_);

  std::strcpy(hostname_, NAMEPREFIX);
  std::strcpy(selfApName_, NAMEPREFIX);
  std::strcpy(hostname_ + NAMEPREFIX_LENGTH, idStr_);
  std::strcpy(selfApName_ + NAMEPREFIX_LENGTH, idStr_);

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
  //   strcpy(wifiSsid_, "OnePlus 10 Pro 5G");
  //   strcpy(wifiPassword_, "rootroot");
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

void Configuration::debugPrintGears() {
  Serial.print("Gears in system: ");
  Serial.println(gearList_.size());

  for (auto pGear : gearList_) {
    Serial.println(pGear->getName());
  }
  Serial.println("-------");
}

void Configuration::getGearNameListString(char *buf) const {
  buf[0] = '\0';

  auto iter = gearList_.begin();
  //   if (iter == gearList_.end()) {
  //     return;
  //   }

  //   std::strcat(buf, "'");
  //   std::strcat(buf, (*iter)->getName());
  //   std::strcat(buf, "'");
  //   iter++;

  while (iter != gearList_.end()) {
    std::strcat(buf, ",'");
    std::strcat(buf, (*iter)->getName());
    std::strcat(buf, "'");
    iter++;
  }
}

} // namespace cooboc