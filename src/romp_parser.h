#ifndef __ROMP_PARSER_H__
#define __ROMP_PARSER_H__

#include "configuration.h"
#include "data_def.h"
#include <cstdint>

namespace cooboc {

using ServerRequestOpt = optional<ServerRequest>;

class RompParser {
public:
  RompParser(const Configuration &configuration);
  ~RompParser() = default;
  ServerRequestOpt prase(std::uint8_t *data, std::size_t len);

private:
  const Configuration &configuration_;
};
} // namespace cooboc

#endif
