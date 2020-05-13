#pragma once
#include "hardware/timing.h"
#include "hardware/input_device.h"
#include "hardware/relay.h"
#include "hardware/gyroscope.h"

namespace ams
{
	namespace logic
	{
		class stimulator_app
		{
		public:
			stimulator_app();
			virtual void run();
			virtual ~stimulator_app();
		protected:
			hardware::timing* timing_;
			hardware::input_device* input_;
			hardware::relay* relay_;
			hardware::gyroscope* gyroscope_;
		};
	}
}
