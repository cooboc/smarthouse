#ifndef __DATA_DEF_H__
#define __DATA_DEF_H__

#include <IPAddress.h>
#include <cstdint>
#include <cstring>

namespace cooboc {

const static IPAddress MY_IP(172, 217, 28, 1);
const static IPAddress SUBNET_MASK(255, 255, 0, 0);
constexpr std::uint16_t WEB_PORT{80U};
constexpr std::uint16_t DNS_PORT{53};

struct Persistent {
  char wifiSsid[256] = {0};
  char wifiPassword[256] = {0};
  bool isWifiCredentialExists{false};
  std::uint16_t crc;

  void reset() {
    std::memset(wifiSsid, 0, sizeof(wifiSsid));
    std::memset(wifiPassword, 0, sizeof(wifiPassword));
    isWifiCredentialExists = false;
    crc = 134U; // just a random picked number, to invalidate the CRC.
  }
};
} // namespace cooboc
#endif
