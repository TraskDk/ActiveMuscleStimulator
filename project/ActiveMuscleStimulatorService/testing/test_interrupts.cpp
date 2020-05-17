#include "../hardware/raspberry/timing_wired.h"
#include "../hardware/raspberry/gyroscope_sensehat_b.h"
#include <cstdio>

namespace ams
{
	namespace testing
	{		
		int test_interrupts(int rate)
		{
			hardware::raspberry::timing_wired timing{};
			hardware::raspberry::gyroscope_sensehat_b_settings settings;
			
			settings.sampling_rate = static_cast<float>(rate);
			settings.accel_scale = hardware::raspberry::accel_scale_4G;
			settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
			hardware::raspberry::gyroscope_sensehat_b gyroscope(settings);
			printf("Using sampling rate of %3.1f.\n", settings.sampling_rate);
			
			movement::movement_vector vec{};
			auto num_checks = 0;
			auto num_vectors = 0;
			const auto start_millis = timing.millis();
			while (true)
			{
				num_checks++;
				if (!gyroscope.read(vec)) continue;
				num_vectors++;
				if ((num_vectors & 127) == 0)
				{
					const auto seconds = (timing.millis() - start_millis) * 0.001;
					const auto rate = (num_vectors-1) / seconds;
					printf("\rvec=%d ch=%d t=%3.1f rate=%3.1f", num_vectors, num_checks, seconds, rate);
					fflush(stdout);
				}
			}
		}
	}
}
