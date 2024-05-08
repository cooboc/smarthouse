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

// Change this value if you want to invalidate Flash
constexpr std::uint8_t CLEAN_FLASH_VALUE{8U};

struct Persistent {
  char wifiSsid[64] = {0};
  char wifiPassword[64] = {0};
  char serverAddr[64] = {0};
  std::uint8_t gearTypeId{-1};
  std::uint8_t gearConfig[64] = {0};

  std::uint8_t dirty = 1U; // The data field used to invalidate data in Flash
  std::uint16_t crc = 32U;

  void reset() {
    std::memset(wifiSsid, 0, sizeof(wifiSsid));
    std::memset(wifiPassword, 0, sizeof(wifiPassword));
    std::memset(serverAddr, 0, sizeof(serverAddr));
    std::memset(gearConfig, 0, sizeof(gearConfig));

    gearTypeId = -1;

    dirty = CLEAN_FLASH_VALUE;
    crc = 134U; // just a random picked number, to invalidate the CRC.
  }
};

template <typename T> struct optional {
  bool hasData = false;
  T data;
};

struct ServerRequest {
  enum class RequestType : std::uint8_t {
    RELAY_ON,
    RELAY_OFF,
    RELAY_ON_LOCK,
    RELAY_OFF_LOCK,
    INVALID,
  };

  std::uint8_t portId{0U};
  RequestType request{RequestType::INVALID};
};

} // namespace cooboc
#endif
