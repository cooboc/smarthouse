#include "configuration.h"
#include "data_def.h"
#include <EEPROM.h>
#include <string.h>

namespace cooboc {
Configuration::Configuration() {}
void Configuration::begin() {
  EEPROM.begin(sizeof(Persistent));

  // TODO: CHANGEME
  // strcpy(wifiSsid_, "astrolek");
  // strcpy(wifiPassword_, "rootroot");
  strcpy(wifiSsid_, "OnePlus 10 Pro 5G");
  strcpy(wifiPassword_, "rootroot");
}
} // namespace cooboc