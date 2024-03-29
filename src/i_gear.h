#ifndef __I_GEAR_H__
#define __I_GEAR_H__

namespace cooboc {
class IGear {
public:
  virtual ~IGear() {}
  virtual const char *getName() const = 0;
};
} // namespace cooboc

#endif
