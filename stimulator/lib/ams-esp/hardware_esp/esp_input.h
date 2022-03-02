#pragma once
#include "hardware/input_device.h"

namespace ams::hardware
{
    class esp_input : public input_device
    {
    public:
        esp_input();
        virtual ~esp_input();

        virtual bool is_available();
		virtual bool get_event(input_key& button, int& state, const bool block = false);
    };
}