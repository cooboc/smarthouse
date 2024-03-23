#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

namespace cooboc
{
    class Configuration
    {
    public:
        Configuration();
        void begin();

        inline const char *getWifiSsid(void) const { return wifiSsid_; }
        inline const char *getWifiPassword(void) const { return wifiPassword_; }

    private:
        char wifiSsid_[256];
        char wifiPassword_[256];
    };

}

#endif