#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include "configuration.h"
#include <cstdint>

namespace cooboc
{
    class StateMachine
    {
    public:
        enum class State : std::uint8_t
        {
            FAILSAFE,
            WORKING_STANDALONE,
            CONNECTING,
        };

        StateMachine(const Configuration &configruation);
        void begin();
        void tick();

    private:
        State state_{State::CONNECTING};
        const Configuration &configuration_;

        void transitOut();
        void transitTo(State newState);
    };

}

#endif
