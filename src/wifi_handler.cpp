#include "wifi_handler.h"
#include "data_def.h"

namespace cooboc {

namespace {} // namespace

WifiHandler::WifiHandler(Configuration &configuration)
    : configuration_(configuration), webPortal_{configuration},
      isWebPortalStarted_{false} {}

void WifiHandler::startConnectAp(const char *ssid, const char *password) const {
  // TODO need to check current status, stop current status

  Serial.print("begin to connect wifi : ");
  Serial.print(ssid);
  Serial.print(" / ");
  Serial.println(password);
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

void WifiHandler::startWebPortal() {
  Serial.println("ready to start web portal");
  Serial.print("AP name");
  Serial.print(strlen(configuration_.getSelfAPName()));
  Serial.print(" :");
  Serial.println(configuration_.getSelfAPName());

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(MY_IP, MY_IP, SUBNET_MASK);
  WiFi.softAP(configuration_.getSelfAPName());

  webPortal_.begin();

  isWebPortalStarted_ = true;
}

void WifiHandler::tick() {
  if (isWebPortalStarted_) {
    webPortal_.tick();
  }
}

} // namespace cooboc