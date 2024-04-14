#include "button3_gear.h"

#include <vector>

namespace cooboc {

static constexpr char GEAR_NAME[]{"3-Button"};
static const PinList PIN_LIST{12U, 14U, 5U};

const char *Button3Gear::getName() const { return GEAR_NAME; }

const PinList &Button3Gear::getInputPins() const { return PIN_LIST; }
} // namespace cooboc
