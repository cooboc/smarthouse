#ifndef __WEB_PORTAL_H__
#define __WEB_PORTAL_H__

#include "configuration.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>

namespace cooboc {
class WebPortal {

public:
  WebPortal(Configuration &configuration);
  void begin();
  void tick();

private:
  Configuration &configuration_;
  DNSServer dnsServer_;
  ESP8266WebServer webServer_;

  void handleWebRoot(void);
  void handleWebSetting(void);
};

} // namespace cooboc

#endif