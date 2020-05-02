#pragma once
#include "relay.h"
#include "timing.h"

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