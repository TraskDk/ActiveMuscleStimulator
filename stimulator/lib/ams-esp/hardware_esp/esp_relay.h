#pragma once

#include "hardware/relay.h"

namespace ams::hardware
{
    class esp_relay : public relay
    {
    public:
        esp_relay();
        virtual ~esp_relay();
	    virtual void set(int channel, bool state);
    };
}