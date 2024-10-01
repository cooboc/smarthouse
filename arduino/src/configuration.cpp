#include "configuration.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <string.h>
#include <cstdint>
#include "button3_gear.h"
#include "data_def.h"
#include "kitchen_button_gear.h"
#include "n_button_relay_gear.h"
#include "test_gear.h"
#include "utils.h"

namespace cooboc {
namespace {

static constexpr char CHARMAP[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void makePrintableId(std::uint32_t id, char buf[7]) {
    constexpr std::size_t CHARMAP_LENGTH = sizeof(CHARMAP) - 1U;
    for (std::size_t i {0U}; i < 6; ++i) {
        buf[5U - i] = id % CHARMAP_LENGTH;
        id /= CHARMAP_LENGTH;
    }

    if (id > 0) {
        buf[0] += id;
        buf[0] %= CHARMAP_LENGTH;
    }
    for (std::size_t i {0U}; i < 6U; ++i) { buf[i] = CHARMAP[buf[i]]; }
    buf[6] = '\0';
}
constexpr std::size_t PERSISTENT_LENGTH = sizeof(Persistent);

}    // namespace

Configuration::Configuration() :
    gearList_ {new TestGear(), new Button3Gear(), new KitchenButtonGear()},
    gearInstance_ {nullptr} {}
void Configuration::begin() {
    debugPrintGears();
    deviceId_                                = ESP.getChipId();
    constexpr char NAME_PREFIX[]             = "csg-";
    constexpr std::size_t NAME_PREFIX_LENGTH = sizeof(NAME_PREFIX) - 1U;
    makePrintableId(deviceId_, idStr_);

    std::strcpy(hostname_, NAME_PREFIX);
    std::strcpy(selfApName_, NAME_PREFIX);
    std::strcpy(hostname_ + NAME_PREFIX_LENGTH, idStr_);
    std::strcpy(selfApName_ + NAME_PREFIX_LENGTH, idStr_);

    // Read EEPROM to memory
    const bool isPersistentHealthy = readFromEEPROM();
    if (!isPersistentHealthy) {
        Serial.println("first start, reset persistent.");
        persistent_.reset();

        // TODO: CHANGEME: for debug only
        std::strcpy(persistent_.wifiSsid, "astrolek");
        std::strcpy(persistent_.wifiPassword, "rootroot");
        std::strcpy(persistent_.serverAddr, "10.1.99.60");
        persistent_.gearTypeId = 1U;    // set to 3 button
        // persistent_.gearConfig[0] = 3U; // connect button 0 & 1
        persistent_.gearConfig[0] = 7U;    // connect button 0 & 1
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

    const std::uint8_t &id = persistent_.gearTypeId;
    Serial.print("gear id: ");
    Serial.println(id);
    if ((id >= 0U) && (id < gearList_.size())) {
        const IGear *gear = gearList_.at(id);
        if (gear != nullptr) {
            gearInstance_ = gear->setupInstance(persistent_.gearConfig);
        }
    }
}

bool Configuration::readFromEEPROM() {
    EEPROM.begin(PERSISTENT_LENGTH);
    std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
    for (std::size_t i {0U}; i < PERSISTENT_LENGTH; ++i) { targetAddr[i] = EEPROM.read(i); }
    EEPROM.end();
    std::uint16_t readoutCrc = persistent_.crc;
    persistent_.dirty        = CLEAN_FLASH_VALUE;
    persistent_.crc          = 0;
    std::uint16_t expectCrc  = utils::calculateCrc(targetAddr, PERSISTENT_LENGTH);
    Serial.print("Readout crc: ");
    Serial.print(readoutCrc);
    Serial.print(" ,expect CRC: ");
    Serial.println(expectCrc);
    return (expectCrc == readoutCrc) && (persistent_.dirty == CLEAN_FLASH_VALUE);
}

void Configuration::writeToEEPROM() {
    std::uint8_t *targetAddr = (std::uint8_t *)(&persistent_);
    persistent_.crc          = 0U;
    std::uint16_t crc        = utils::calculateCrc(targetAddr, PERSISTENT_LENGTH);
    persistent_.crc          = crc;
    EEPROM.begin(PERSISTENT_LENGTH);
    for (std::size_t i {0U}; i < PERSISTENT_LENGTH; ++i) { EEPROM.write(i, targetAddr[i]); }
    bool success = EEPROM.commit();
    EEPROM.end();
    Serial.println(success ? "Update EEPROM successful" : "Update EEPROM failed");
}

void Configuration::debugPrintGears() {
    Serial.print("Gears in system: ");
    Serial.println(gearList_.size());

    for (auto pGear : gearList_) { Serial.println(pGear->getName()); }
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

void Configuration::updateConfiguration(const char *ssid,
                                        const char *password,
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

void Configuration::updateConnectivity(std::uint8_t c) {
    persistent_.gearConfig[0U] = c;
}

void Configuration::commitUpdate() {
    writeToEEPROM();
}

void Configuration::resetSettings() {
    persistent_.reset();
    commitUpdate();
}

}    // namespace cooboc