#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "data_def.h"

namespace cooboc {
class Configuration {
public:
  Configuration();
  void begin();

  inline const char *getWifiSsid(void) const { return wifiSsid_; }
  inline const char *getWifiPassword(void) const { return wifiPassword_; }

private:
  Persistent persistent_;
  char wifiSsid_[256];
  char wifiPassword_[256];

  bool readFromEEPROM();
  void writeToEEPROM();
};

} // namespace cooboc

#endif