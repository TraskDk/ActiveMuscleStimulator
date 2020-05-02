#include "relay_sbz4.h"
#include <wiringPi.h>

namespace ams
{
	namespace hardware
	{
		static const int num_channels = 4;
		static const int gpio_channel_pins[] = { 22, 23, 24, 25 };
	
		relay_sbz4::relay_sbz4() : current_states_{}
		{
			reset();
		}

		relay_sbz4::~relay_sbz4()
		{
			reset();
		}

		void relay_sbz4::reset()
		{
			for (auto i = 0; i < 4; i++)
			{
				const auto pin = gpio_channel_pins[i];
				pinMode(pin, OUTPUT);
				digitalWrite(pin, LOW);
				current_states_[i] = false;
			}
		}
		
		void relay_sbz4::set(const int channel, const bool state)
		{
			if (channel < 0 || channel >= num_channels) return;
			if (current_states_[channel] == state) return;
			const auto pin = gpio_channel_pins[channel];
			pinMode(pin, OUTPUT);
			digitalWrite(pin, state ? HIGH : LOW);
			current_states_[channel] = state;
		}

	}
}

