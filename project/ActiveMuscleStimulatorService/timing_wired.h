#pragma once
#include <stdexcept>
#include "timing.h"
#include "wiringPi.h"

namespace ams
{
	namespace hardware
	{
		class timing_wired : public timing
		{
		public:
			timing_wired()
			{
				if (wiringPiSetup() == -1)
					throw std::runtime_error("Unable to setup wiringPi.");
				piHiPri(99);
			}
			unsigned micros() override;
			void delayMicroseconds(unsigned howLong) override;
		};
	}
}