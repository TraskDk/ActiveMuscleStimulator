#include "timing_wired.h"
#include <wiringPi.h>

namespace ams
{
	namespace hardware
	{
		
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