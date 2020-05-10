#include "hardware/raspberry/relay_sbz4.h"
#include "hardware/raspberry/gyroscope_sensehat_b.h"
#include "hardware/gyroscope_smoothing.h"
#include "hardware/cpu_monitor.h"
#include "hardware/timing_wired.h"
#include <wiringPi.h>
#include <cstdio>
#include <stdexcept>

using namespace ams;

int main(int argc, const char ** argv)
{
	try
	{
		hardware::timing_wired timing;
		hardware::raspberry::relay_sbz4 relay;

		//testing::music::play(&timing, &relay);

		for (auto it = 0; it < 2; it++)
		{
			printf("Initializing hardware.\n");
			hardware::raspberry::gyroscope_sensehat_b_settings settings{};
			settings.accel_scale = hardware::raspberry::accel_scale_8G;
			settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
			settings.sampling_rate = 100;
			const auto numIterations = 500;

			//hardware::raspberry::gyroscope_sensehat_b gyro_raw(settings);
			//hardware::gyroscope_smoothing gyro(0.001f, &gyro_raw);
			hardware::raspberry::gyroscope_sensehat_b gyro(settings);	
			
			delay(100);
			printf("Ready.\n");

			hardware::cpu_monitor cpu_mon;
			
			movement::movement_vector vec{};
			const auto start = millis();

			auto last_call = micros();
			auto numWaits = 0;

			const auto duration = static_cast<int>(1000000 / (1.1f * settings.sampling_rate));
			for (auto i = 1; i <= numIterations; i++)
			{
				if (!gyro.read(vec))
				{
					numWaits++;
					const auto elapsed = micros() - last_call;
					const auto remaining = duration - static_cast<int>(elapsed);
					if (remaining > 0)
						delayMicroseconds(remaining);
					while (!gyro.read(vec))
						delayMicroseconds(500);
				}
				last_call = micros();
			}
			const auto seconds = static_cast<float>(millis() - start) / 1000.0f;
			const auto rate = numIterations / seconds;

			cpu_mon.print_usage();
			{
				printf("%3.1f\t%d\t%5.2f\t%5.2f\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",
					rate, numWaits,
					vec.values[0], vec.values[1], vec.values[2],
					vec.values[3], vec.values[4], vec.values[5]);
			}

			printf("Done.\n");
			delay(1000);
		}
	}
	catch(const std::exception & ex)
	{
		printf("ERROR: %s\n", ex.what());
		return 1;
	}
	return 0;
}