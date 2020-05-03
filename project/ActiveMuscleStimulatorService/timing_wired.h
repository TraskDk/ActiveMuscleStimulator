#pragma once
#include <stdexcept>
#include "timing.h"
#include "wiringPi.h"

namespace ams
{
	namespace hardware
	{
		
		/**
		 * \brief An implementation of the `timing` class, which is based on the logic from `wiringPi`. 
		 */
		class timing_wired : public timing
		{
		public:
			/**
			 * \brief Initializing `wiringPi`.
			 */
			timing_wired();
			/**
			 * \brief Gets the current time at microsecond granularity.
			 * \return The current time.
			 */
			unsigned micros() override;
			/**
			 * \brief Blocks the current thread the specified number of microseconds.
			 * \param howLong The number of microseconds to block.
			 */
			void delayMicroseconds(unsigned howLong) override;
		};
	}
}