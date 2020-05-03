#include "timing_wired.h"
#include <wiringPi.h>

namespace ams
{
	namespace hardware
	{
		timing_wired::timing_wired()
		{
			if (wiringPiSetup() == -1)
				throw std::runtime_error("Unable to setup wiringPi.");
			piHiPri(99);
		}

		unsigned timing_wired::micros()
		{
			return ::micros();
		}
		
		void timing_wired::delayMicroseconds(const unsigned howLong)
		{
			 ::delayMicroseconds(howLong);
		}
	}
}