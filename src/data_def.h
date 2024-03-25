#ifndef __DATA_DEF_H__
#define __DATA_DEF_H__

#include <cstdint>
#include <cstring>

namespace cooboc {
struct Persistent {
  char wifiSsid[256] = {0};
  char wifiPassword[256] = {0};
  std::uint16_t crc;

  void reset() {
    std::memset(wifiSsid, 0, sizeof(wifiSsid));
    std::memset(wifiPassword, 0, sizeof(wifiPassword));
    crc = 134U; // just a random picked number, to invalidate the CRC.
  }
};
} // namespace cooboc
#endif