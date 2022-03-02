#pragma once
#include "hardware/timing.h"

namespace ams::hardware
{
    class esp_timing : timing {
    public:
        esp_timing(); 
        virtual ~esp_timing();
        virtual unsigned micros();
        virtual void delayMicroseconds(unsigned howLong);
        virtual unsigned millis();
        virtual void delayMilliseconds(unsigned howLong);
    };
}