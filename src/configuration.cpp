#include "configuration.h"
#include "button3_gear.h"
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
constexpr std::size_t PERSISTENT_LENGTH = sizeof(Persistent);

} // namespace

Configuration::Configuration() : gearList_{new TestGear(), new Button3Gear()} {}
void Configuration::begin() {
  debugPrintGears();
  deviceId_ = ESP.getChipId();
  constexpr char NAMEPREFIX[] = "csg-";
  constexpr std::size_t NAMEPREFIX_LENGTH = sizeof(NAMEPREFIX) - 1U;
  makePrintableId(deviceId_, idStr_);

  std::strcpy(hostname_, NAMEPREFIX);
  std::strcpy(selfApName_, NAMEPREFIX);
  std::strcpy(hostname_ + NAMEPREFIX_LENGTH, idStr_);
  std::strcpy(selfApName_ + NAMEPREFIX_LENGTH, idStr_);

  // Read EEPROM to memory
  const bool isPersistentHealthy = readFromEEPROM();
  if (!isPersistentHealthy) {
    Serial.println("first start, reset persistent.");
    persistent_.reset();

    // TODO: CHANGEME: for debug only
    std::strcpy(persistent_.wifiSsid, "astrolek");
    std::strcpy(persistent_.wifiPassword, "rootroot");
    std::strcpy(persistent_.serverAddr, "10.1.99.60");
    persistent_.gearTypeId = 1U;
    writeToEEPROM();
  } else {
    Serial.println("EEPROM good!");
    Serial.print("wifi in persistent: ");
    Serial.print(persistent_.wifiSsid);
    Serial.print(", crc=");
    Serial.println(persistent_.crc);
  }

  // TODO: CHANGEME
  // strcpy(wifiSsid_, "astrolek");
  // strcpy(wifiPassword_, "rootroot");
  //   strcpy(wifiSsid_, "OnePlus 10 Pro 5G");
  //   strcpy(wifiPassword_, "rootroot");
}

bool Configuration::readFromEEPROM() {
  EEPROM.begin(PERSISTENT_LENGTH);
  std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
  for (std::size_t i{0U}; i < PERSISTENT_LENGTH; ++i) {
    targetAddr[i] = EEPROM.read(i);
  }
  EEPROM.end();
  std::uint16_t readoutCrc = persistent_.crc;
  persistent_.crc = 0;
  std::uint16_t expectCrc = utils::calculateCrc(targetAddr, PERSISTENT_LENGTH);
  Serial.print("Readout crc: ");
  Serial.print(readoutCrc);
  Serial.print(" ,expect CRC: ");
  Serial.println(expectCrc);
  return (expectCrc == readoutCrc) && (persistent_.dirty == CLEAN_FLASH_VALUE);
}

void Configuration::writeToEEPROM() {
  std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
  persistent_.crc = 0U;
  std::uint16_t crc = utils::calculateCrc(targetAddr, PERSISTENT_LENGTH);
  persistent_.crc = crc;
  EEPROM.begin(PERSISTENT_LENGTH);
  for (std::size_t i{0U}; i < PERSISTENT_LENGTH; ++i) {
    EEPROM.write(i, targetAddr[i]);
  }
  bool success = EEPROM.commit();
  EEPROM.end();
  Serial.println(success ? "Update EEPROM successful" : "Update EEPROM failed");
}

void Configuration::debugPrintGears() {
  Serial.print("Gears in system: ");
  Serial.println(gearList_.size());

  for (auto pGear : gearList_) {
    Serial.println(pGear->getName());
  }
  Serial.println("-------");
}

const IGear *Configuration::getGear() const {
  const auto id = persistent_.gearTypeId;
  if (id >= 0U && id < gearList_.size()) {
    return gearList_.at(id);
  }
  return nullptr;
};

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

void Configuration::updateConfiguration(const char *ssid, const char *password,
                                        const char *serverAddr,
                                        const std::uint8_t id) {
  std::strcpy(persistent_.wifiSsid, ssid);
  std::strcpy(persistent_.wifiPassword, password);
  std::strcpy(persistent_.serverAddr, serverAddr);
  persistent_.gearTypeId = id;
  writeToEEPROM();
}

void Configuration::updateWifiSsid(const char *ssid) {
  std::strcpy(persistent_.wifiSsid, ssid);
}
void Configuration::updateWifiPassword(const char *password) {
  std::strcpy(persistent_.wifiPassword, password);
}

void Configuration::updateServerAddr(const char *serverAddr) {
  std::strcpy(persistent_.serverAddr, serverAddr);
}
void Configuration::updateGearTypeId(const std::uint8_t id) {
  persistent_.gearTypeId = id;
}
void Configuration::commitUpdate() { writeToEEPROM(); }

} // namespace cooboc