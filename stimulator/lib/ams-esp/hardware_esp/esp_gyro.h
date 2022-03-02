#pragma once
#include "hardware/gyroscope.h"

namespace ams::hardware 
{
    class esp_gyro : public gyroscope {
    public:
        esp_gyro();
        virtual ~esp_gyro();
        bool read(movement::movement_vector& vec);
    };
}