#include "esp_input.h"

namespace ams::hardware
{
    esp_input::esp_input()
    {
    }

    esp_input::~esp_input()
    {
    }

    bool esp_input::is_available()
    {
        return false;
    }

    bool esp_input::get_event(input_key& button, int& state, const bool block)
    {
        return false;
    }
}