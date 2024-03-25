#include "wifi_handler.h"

namespace cooboc {
WifiHandler::WifiHandler() {}

void WifiHandler::startConnectAp(const char *ssid, const char *password) const {
  // TODO need to check current status, stop current status

  Serial.println("begin to connect wifi");
  // Wifi.begin
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

WifiHandler::WifiStatus WifiHandler::getWifiStatus() const {
  wl_status_t status = WiFi.status();
  /*
  WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
  WL_IDLE_STATUS      = 0,
  WL_NO_SSID_AVAIL    = 1,
  WL_SCAN_COMPLETED   = 2,
  WL_CONNECTED        = 3,
  WL_CONNECT_FAILED   = 4,
  WL_CONNECTION_LOST  = 5,
  WL_WRONG_PASSWORD   = 6,
  WL_DISCONNECTED     = 7
  */
  switch (status) {
  case (WL_IDLE_STATUS): {
    return WifiStatus::IDLE;
  }
  case (WL_DISCONNECTED): {
    return WifiStatus::CONNECTING;
  }
  case (WL_CONNECTED): {
    return WifiStatus::CONNECTED;
  }
  default:
    Serial.print("unknown status: ");
    Serial.println((int)(status));
    delay(100);
    return WifiStatus::UNKNOWN;
  }
}

} // namespace cooboc