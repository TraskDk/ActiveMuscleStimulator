#include "timing_wired.h"
#include <wiringPi.h>
#include <stdexcept>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
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

			unsigned timing_wired::millis()
			{
				return ::millis();
			}

			void timing_wired::delayMilliseconds(unsigned howLong)
			{
				::delay(howLong);
			}
		}
	}
}