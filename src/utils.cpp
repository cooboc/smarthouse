#include "utils.h"
#include <cstdint>

namespace cooboc {
std::uint16_t calculateCrc(std::uint8_t *dataPtr, std::size_t len) {
  std::uint16_t crc = 0;
  for (std::size_t i = 0U; i < len; ++i) {
    std::uint16_t data = *(dataPtr + i);
    crc ^= data;
    if (crc & 0x01) {
      crc = (crc << 1) ^ 0xA001;
    } else {
      crc = (crc << 1);
    }
  }
  return crc;
}

} // namespace cooboc