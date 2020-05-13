#include "stimulator_app.h"
#include "../hardware/raspberry/input_controller.h"
#include "../hardware/raspberry/relay_sbz4.h"
#include "../hardware/raspberry/gyroscope_sensehat_b.h"
#include "../hardware/raspberry/timing_wired.h"

namespace ams
{
	namespace logic
	{
		static const float gyro_sampling_rate = 120;
		
		stimulator_app::stimulator_app()
		{
			timing_ = new hardware::raspberry::timing_wired();
			input_ = new hardware::raspberry::input_controller();
			relay_ = new hardware::raspberry::relay_sbz4();
			hardware::raspberry::gyroscope_sensehat_b_settings settings;
			settings.sampling_rate = gyro_sampling_rate;
			settings.accel_scale = hardware::raspberry::accel_scale_4G;
			settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
			gyroscope_ = new hardware::raspberry::gyroscope_sensehat_b(settings);
		}
		
		void stimulator_app::run()
		{

		}

		stimulator_app::~stimulator_app()
		{
			if(gyroscope_ != nullptr)
			{
				delete gyroscope_;
				gyroscope_ = nullptr;
			}

			if (relay_ != nullptr)
			{
				delete relay_;
				relay_ = nullptr;
			}

			if (input_ != nullptr)
			{
				delete input_;
				input_ = nullptr;
			}
			if (timing_ != nullptr)
			{
				delete timing_;
				timing_ = nullptr;
			}
		}
	}
}
