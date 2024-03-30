#ifndef __WIFI_HANDLER_H__
#define __WIFI_HANDLER_H__

#include "configuration.h"
#include "web_portal.h"
#include <ESP8266WiFi.h>
#include <cstdint>

namespace cooboc {
class WifiHandler {
public:
  enum class WifiStatus : uint8_t {
    UNKNOWN,
    IDLE, // Wifi idle, not begin
    CONNECTING,
    CONNECTED,
    WRONG_AP,
  };
  WifiHandler(Configuration &configuration_);
  void startWebPortal();
  void startConnectAp(const char *ssid, const char *password) const;
  WifiHandler::WifiStatus getWifiStatus() const;

  void tick();

private:
  const Configuration &configuration_;
  WebPortal webPortal_;
  bool isWebPortalStarted_;
};
} // namespace cooboc

#endif
