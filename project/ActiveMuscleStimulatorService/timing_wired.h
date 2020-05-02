#pragma once
#include "timing.h"

namespace ams
{
	namespace hardware
	{
		class timing_wired : public timing
		{
		public:
			unsigned micros() override;
			void delayMicroseconds(unsigned howLong) override;
		};
	}
}