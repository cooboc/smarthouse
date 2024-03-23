#include "configuration.h"
#include <string.h>

namespace cooboc
{
    Configuration::Configuration() {}
    void Configuration::begin()
    {
        // TODO: CHANGEME
        strcpy(wifiSsid_, "astrolek");
        strcpy(wifiPassword_, "rootroot");
    }
}