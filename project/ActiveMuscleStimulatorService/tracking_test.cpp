#include "hardware/raspberry/gyroscope_sensehat_b.h"
#include "hardware/gyroscope_smoothing.h"
#include "hardware/cpu_monitor.h"
#include "hardware/timing_wired.h"
#include "movement/symmetric/symmetric_movement_translator.h"
#include "movement/sample_point_log.h"
#include "collections/smart_list.h"
#include <wiringPi.h>
#include <cstdio>
#include <stdexcept>

using namespace ams;

int tracking_test(const char * learning_file)
{
	try
	{
		hardware::timing_wired timing;

		printf("Initializing hardware.\n");
		hardware::raspberry::gyroscope_sensehat_b_settings settings{};
		settings.accel_scale = hardware::raspberry::accel_scale_8G;
		settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
		settings.sampling_rate = 120;
		const auto numIterations = 1500;

		hardware::raspberry::gyroscope_sensehat_b gyro_raw(settings);
		hardware::gyroscope_smoothing gyro(0.01f, &gyro_raw);

		hardware::cpu_monitor cpu_mon;
		movement::movement_translator* translator = nullptr;
		{
			collections::smart_list<movement::sample_point> points;
			movement::sample_point_log::read(learning_file, points);
			translator = movement::create_symmetric_movement_translator(points);
		}
		
		movement::movement_vector vec{};
		movement::movement_activation act{};
		const auto start = millis();
		const auto start_us = micros();

		auto last_call = micros();
		auto numWaits = 0;
		auto numTracked = 0;

		printf("Tracking movements.\n");
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


			float progress, likelihood;
			if (translator->observe(last_call - start_us, vec, act, progress, likelihood))
			{
				numTracked++;
			}
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

		printf("Tracked %d out of %d movement vectors.\n", numTracked, numIterations);
		
		printf("Done.\n");
	}
	catch (const std::exception& ex)
	{
		printf("ERROR: %s\n", ex.what());
		return 1;
	}
	return 0;
}