#include "../hardware/raspberry/timing_wired.h"
#include "../hardware/raspberry/gyroscope_sensehat_b.h"
#include "../hardware/raspberry/interrupt_handler.h"
#include <cstdio>

namespace ams
{
	namespace testing
	{
		static const int interrupt_pin = 6;
						
		int test_interrupts()
		{
			hardware::raspberry::interrupt_handler irq(interrupt_pin);
			hardware::raspberry::timing_wired timing{};
			hardware::raspberry::gyroscope_sensehat_b_settings settings;
			
			settings.sampling_rate = 120;
			settings.accel_scale = hardware::raspberry::accel_scale_4G;
			settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
			hardware::raspberry::gyroscope_sensehat_b gyroscope(settings);

			movement::movement_vector vec{};
			auto num_interrupts = 0;
			auto num_checks = 0;
			auto num_vectors = 0;
			while (true)
			{
				if(irq.wait(1000))
					num_interrupts++;

				//has_data = false;
				num_checks++;
				if (!gyroscope.read(vec)) continue;
				num_vectors++;
				if ((num_vectors & 31) == 0)
				{
					printf("\rvec=%d ir=%d ch=%d", num_vectors, num_interrupts, num_checks);
					fflush(stdout);
				}
			}
		}
	}
}
