#include "test_gear.h"

namespace cooboc {

static constexpr char GEAR_NAME[]{"TEST"};

const char *TestGear::getName() const { return GEAR_NAME; }

} // namespace cooboc