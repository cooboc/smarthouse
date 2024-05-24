#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "data_def.h"
#include "i_gear.h"
#include <array>
#include <cstring>

namespace cooboc {
class Configuration {
public:
  Configuration();
  void begin();

  inline const char *getHostname(void) const { return hostname_; }
  inline const char *getSelfAPName(void) const { return selfApName_; }
  inline const char *getPrintableId(void) const { return idStr_; }
  inline uint32_t getDeviceId(void) const { return deviceId_; }
  inline uint8_t getGearTypeId(void) const { return persistent_.gearTypeId; }
  void getGearNameListString(char *buf) const;
  IGearInstance *getGearInstance(void) const { return gearInstance_; }

  void updateConfiguration(const char *ssid, const char *password,
                           const char *serverAddr, std::uint8_t id);
  void updateWifiSsid(const char *ssid);
  void updateWifiPassword(const char *password);
  void updateServerAddr(const char *serverAddr);
  void updateGearTypeId(std::uint8_t id);
  void commitUpdate();
  void resetSettings();

  bool isWifiCredentialExists() const {
    return (std::strlen(persistent_.wifiSsid) > 0);
  }

  inline const char *getWifiSsid(void) const { return persistent_.wifiSsid; }
  inline const char *getWifiPassword(void) const {
    return persistent_.wifiPassword;
  }
  inline const char *getServerAddr(void) const {
    return persistent_.serverAddr;
  }

private:
  uint32_t deviceId_{0U};
  char idStr_[7];       // 123456
  char hostname_[12];   //"csg-[6]";
  char selfApName_[12]; //"csg-[6]", csg-123456;

  Persistent persistent_;
  std::array<const IGear *, 3U> gearList_;
  IGearInstance *gearInstance_;
  bool readFromEEPROM();
  void writeToEEPROM();

  void debugPrintGears();
};

} // namespace cooboc

#endif