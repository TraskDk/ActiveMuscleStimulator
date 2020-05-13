#include "hardware/raspberry/relay_sbz4.h"
#include "hardware/raspberry/input_controller.h"
#include <wiringPi.h>
#include <cstdio>

namespace ams
{
	using namespace hardware::raspberry;

	void test_controller()
	{
		wiringPiSetup();
		relay_sbz4 relay;
		input_controller ctrl;

		int button, state;

		for (auto i = 0;; i++)
		{
			if ((i & ((1 << 15) - 1)) == 0)
			{
				printf("\r%d", i);
				fflush(stdout);
			}

			if (ctrl.get_event(button, state))
			{
				switch (button)
				{
				case key_a: relay.set(0, state > 0); break;
				case key_b: relay.set(1, state > 0); break;
				case key_x: relay.set(2, state > 0); break;
				case key_y: relay.set(3, state > 0); break;
				default: break;
				}
			}
		}
	}
}