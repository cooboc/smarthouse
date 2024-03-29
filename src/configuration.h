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

  inline const bool isWifiCredentialExists(void) const {
    return this->persistent_.isWifiCredentialExists;
    // return std::strlen(this->persistent_.wifiSsid) > 0;
  }
  inline const char *getHostname(void) const { return hostname_; }
  inline const char *getSelfAPName(void) const { return selfApName_; }
  inline const char *getPrintableId(void) const { return idStr_; }
  void getGearNameListString(char *buf) const;

  // TODO
  inline const char *getWifiSsid(void) const { return persistent_.wifiSsid; }
  // TODO
  inline const char *getWifiPassword(void) const {
    return persistent_.wifiPassword;
  }

private:
  char idStr_[7];       // 123456
  char hostname_[12];   //"csg-[6]";
  char selfApName_[12]; //"csg-[6]", csg-123456;

  Persistent persistent_;
  std::array<const IGear *, 1U> gearList_;
  bool readFromEEPROM();
  void writeToEEPROM();

  void debugPrintGears();
};

} // namespace cooboc

#endif