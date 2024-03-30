#include "web_portal.h"
#include "html_fregments.h"
#include <Arduino.h>
#include <cstdint>
#include <cstring>

namespace cooboc {

WebPortal::WebPortal(const Configuration &configuration)
    : configuration_{configuration}, webServer_{80} {}

void WebPortal::begin() {
  Serial.println("begin to setting up the webportal.");

  // Capture all doname name request, if the doname name set to "*". All
  // request forward to provided IP.
  dnsServer_.start(DNS_PORT, "*", MY_IP);
  // setup the webserver
  webServer_.on("/set", HTTP_POST, [this]() { this->handleWebSetting(); });
  // replay to all requests with same HTML
  // For Microsoft
  webServer_.on("/fwlink", [this]() { this->handleWebRoot(); });
  // For Android device
  webServer_.on("/generate_204", [this]() { this->handleWebRoot(); });
  // For other situation
  webServer_.onNotFound([this]() { this->handleWebRoot(); });
  webServer_.begin(WEB_PORT);
}

void WebPortal::tick() {
  dnsServer_.processNextRequest();
  webServer_.handleClient();
}

void WebPortal::handleWebRoot(void) {
  Serial.println("serve the web root");
  constexpr size_t totalLengthWithoutGear =
      sizeof(htmlHeader1) - 1 + sizeof(htmlHeader3) - 1 +
      sizeof(htmlSetupBody1) - 1 + sizeof(htmlSetupBody2) - 1 +
      sizeof(htmlSetupBody3) - 1 + 6;

  char gearNameList[256];
  configuration_.getGearNameListString(gearNameList);
  const std::size_t gearNameListLength = std::strlen(gearNameList);
  const std::size_t totalLength = totalLengthWithoutGear + gearNameListLength;

  Serial.print("gearNameList length: ");
  Serial.println(gearNameListLength);
  Serial.println(gearNameList);

  webServer_.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer_.sendHeader("Pragma", "no-cache");
  webServer_.sendHeader("Expires", "-1");
  webServer_.setContentLength(totalLength);
  // here begin chunked transfer
  webServer_.send(200, "text/html", "");
  webServer_.sendContent(FPSTR(htmlHeader1));
  webServer_.sendContent(FPSTR(htmlHeader3));
  webServer_.sendContent(FPSTR(htmlSetupBody1));
  webServer_.sendContent(FPSTR(configuration_.getPrintableId()));
  webServer_.sendContent(FPSTR(htmlSetupBody2));
  webServer_.sendContent(gearNameList);
  webServer_.sendContent(FPSTR(htmlSetupBody3));
  webServer_.client().stop();
}
void WebPortal::handleWebSetting(void) {
  static const char updStr[] = "updating...</body></html>";
  constexpr size_t totalLenght = sizeof(htmlHeader1) - 1 + sizeof(htmlHeader2) -
                                 1 + sizeof(htmlHeader3) - 1 + sizeof(updStr) -
                                 1;
  webServer_.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer_.sendHeader("Pragma", "no-cache");
  webServer_.sendHeader("Expires", "-1");
  webServer_.setContentLength(totalLenght);
  webServer_.send(200, "text/html", "");
  webServer_.sendContent(FPSTR(htmlHeader1));
  webServer_.sendContent(FPSTR(htmlHeader2));
  webServer_.sendContent(FPSTR(htmlHeader3));
  webServer_.sendContent(updStr);
  webServer_.client().stop();

  char buf[256];

  // Parse ssid
  webServer_.arg("s").toCharArray(buf, 256);
  buf[255] = '\0';
  char ssid[256];
  std::strcpy(ssid, buf);

  // Parse wifi password
  webServer_.arg("w").toCharArray(buf, 256);
  buf[255] = '\0';
  char password[256];
  std::strcpy(password, buf);

  // Parse host
  webServer_.arg("h").toCharArray(buf, 256);
  buf[255] = '\0';
  char host[256];
  std::strcpy(host, buf);

  // Parse type
  int type = webServer_.arg("t").toInt();

  Serial.println("get config from web portal:");
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
  Serial.print("Host: ");
  Serial.println(host);
  Serial.print("type: ");
  Serial.println(type);
}

} // namespace cooboc