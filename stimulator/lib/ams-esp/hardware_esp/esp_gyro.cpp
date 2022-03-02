#include "esp_gyro.h"

namespace ams::hardware 
{
    esp_gyro::esp_gyro()
    {
    }

    esp_gyro::~esp_gyro()
    {        
    }

    bool esp_gyro::read(movement::movement_vector& vec)
    {
        return false;
    }
}