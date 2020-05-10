#pragma once
#include "hardware/relay.h"
#include "hardware/timing.h"

namespace ams
{
	namespace testing
	{
		class music
		{
		public:
			static void play(hardware::timing * timing, hardware::relay * r);
		};
	}
}