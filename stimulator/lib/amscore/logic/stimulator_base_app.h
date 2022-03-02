#pragma once

#include "hardware/timing.h"
#include "hardware/input_device.h"
#include "hardware/relay.h"
#include "hardware/gyroscope.h"

namespace ams::logic
{
    class stimulator_base_app {
    public:
        virtual ~stimulator_base_app() = default;
        virtual void run() = 0;
    protected:
        virtual bool load_program(int program_index)=0;
        virtual bool save_program(int program_index)=0;
        virtual void program()=0;
        virtual void reset()=0;
        ams::hardware::timing* timing_;
        ams::hardware::input_device* input_;
        ams::hardware::relay* relay_;
        ams::hardware::gyroscope* gyroscope_;
    };
}