#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>

namespace cooboc {

std::uint16_t calculateCrc(std::uint8_t *dataPtr, std::size_t len);

}
#endif
