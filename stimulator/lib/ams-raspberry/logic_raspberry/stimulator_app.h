#pragma once
#include "logic/stimulator_base_app.h"
#include "hardware/timing.h"
#include "hardware/input_device.h"
#include "hardware/relay.h"
#include "hardware/gyroscope.h"
#include "movement/movement_translator.h"

namespace ams
{
	namespace logic
	{		
		class stimulator_app : public stimulator_base_app
		{
		public:
			stimulator_app();
			virtual void run();
			virtual ~stimulator_app();
		protected:
			virtual bool load_program(int program_index);
			virtual bool save_program(int program_index);
			virtual void program();
			virtual void reset();
			hardware::timing* timing_;
			hardware::input_device* input_;
			hardware::relay* relay_;
			hardware::gyroscope* gyroscope_;
		};
	}
}
