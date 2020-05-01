#include "relay.h"
#include <wiringPi.h>

namespace AMS
{
	static const int num_channels = 4;
	static const int channel_pins[] = { 22, 23, 24, 25 };
	
	relay::relay()
	{
		reset();
	}

	void relay::reset()
	{
		for(auto i=0;i<4;i++)
		{
			auto pin = channel_pins[i];
			pinMode(pin, OUTPUT);
			digitalWrite(pin, LOW);
			_current_states[i] = false;
		}
	}
	
	void relay::set(const int channel, const bool state)
	{
		if (channel < 0 || channel >= num_channels) return;
		if (_current_states[channel] == state) return;
		const auto pin = channel_pins[channel];
		pinMode(pin, OUTPUT);
		digitalWrite(pin, state ? HIGH : LOW);
		_current_states[channel] = state;
	}
}