#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>
#include <functional>

namespace cooboc {

namespace utils {
std::uint16_t calculateCrc(std::uint8_t *dataPtr, std::size_t len);
void writeUint32(const std::uint32_t value, std::uint8_t *dataPtr);
void writeUint8(const std::uint8_t value, std::uint8_t *dataPtr);

extern std::function<void(void)> EMPTY_FUNCTION;

} // namespace utils
} // namespace cooboc
#endif
