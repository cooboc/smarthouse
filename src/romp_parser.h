#ifndef __ROMP_PARSER_H__
#define __ROMP_PARSER_H__

#include "data_def.h"
#include <cstdint>

namespace cooboc {

using ServerRequestOpt = optional<ServerRequest>;

class RompParser {
public:
  RompParser() = default;
  ~RompParser() = default;
  ServerRequestOpt prase(std::uint8_t *data, std::size_t len);
};
} // namespace cooboc

#endif
