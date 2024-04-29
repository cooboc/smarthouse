#include "utils.h"
#include <cstdint>

namespace cooboc {
namespace utils {
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

void writeUint32(const std::uint32_t value, std::uint8_t *dataPtr) {
  dataPtr[0] = (std::uint8_t)(value & 0x00FF);
  dataPtr[1] = (std::uint8_t)((value >> 8) & 0x00FF);
  dataPtr[2] = (std::uint8_t)((value >> 16) & 0x00FF);
  dataPtr[3] = (std::uint8_t)((value >> 24) & 0x00FF);
}

void writeUint8(const std::uint8_t value, std::uint8_t *dataPtr) {
  *dataPtr = value;
}

std::function<void(void)> EMPTY_FUNCTION = []() {};

} // namespace utils
} // namespace cooboc
