#ifndef __WIFI_HANDLER_H__
#define __WIFI_HANDLER_H__

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
  WifiHandler();
  void startConnectAp(const char *ssid, const char *password) const;
  WifiHandler::WifiStatus getWifiStatus() const;
  void tick();
};
} // namespace cooboc

#endif
