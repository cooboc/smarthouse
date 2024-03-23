#include "state_machine.h"
#include "configuration.h"

#include <ESP8266WiFi.h>

namespace cooboc
{
    // The reason why I didn't put initializing code in the constructor is I want
    // to keep the Arduino style, a.k.a the setup() and loop() functions. But I
    // don't know what the underneath framework will do when finish a loop() operation.
    // To make the variable visible to both two function. I have to make a global
    // variable. And the constructor of global variable will called before the setup()
    // function is called, and even before the hardware is initialized. It may cause
    // undefined behaviour. To avoid this situation, I have to make this compromise.
    // Move all initialization code to begin() function.
    StateMachine::StateMachine(const Configuration &configuration) : configuration_(configuration)
    {
    }

    void StateMachine::begin()
    {
        transitTo(State::CONNECTING);
    }

    void StateMachine::tick()
    {
        switch (state_)
        {
        case (State::FAILSAFE):
        {
            break;
        }
        case (State::CONNECTING):
        {
            wl_status_t wifiStatus = WiFi.status();
            if (wifiStatus == wl_status_t::WL_CONNECTED)
            {
                transitOut();
                transitTo(State::WORKING_STANDALONE);
            }
            break;
        }
        case (State::WORKING_STANDALONE):
        {
            static unsigned long lastPrintTime = millis();
            if (millis() - lastPrintTime > 1000U)
            {
                Serial.println("working");
                lastPrintTime = millis();
            }
            break;
        }
        default:
        {
            transitTo(State::FAILSAFE);
        }
        }
    }

    void StateMachine::transitOut() {}
    void StateMachine::transitTo(State newState)
    {
        state_ = newState;
        switch (state_)
        {
        case (State::FAILSAFE):
        {
            while (true)
                ;
            break;
        }
        case (State::CONNECTING):
        {
            // Wifi.begin
            WiFi.mode(WIFI_STA);
            WiFi.begin(configuration_.getWifiSsid(), configuration_.getWifiPassword());

            break;
        }
        case (State::WORKING_STANDALONE):
        {
            break;
        }
        default:
        {
            // TODO: LOG an error
            transitTo(State::FAILSAFE);
            break;
        }
        }
    }
}
