#include "test_gear.h"

namespace cooboc {

constexpr char GEAR_NAME[]{"TEST"};

TestGear::TestGear() {}

const char *TestGear::getName() const { return GEAR_NAME; }

} // namespace cooboc