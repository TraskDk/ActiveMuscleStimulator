#pragma once

namespace ams
{
	namespace hardware
	{
		class timing
		{
		public:
			virtual unsigned micros() = 0;
			virtual void delayMicroseconds(unsigned howLong) = 0;
		};
	}
}
