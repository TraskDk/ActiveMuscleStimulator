#pragma once

namespace ams
{
	namespace hardware
	{		
		/**
		 * \brief Abstract that delivers logic related time and delay.
		 */
		class timing
		{
		public:
			/**
			 * \brief Gets the current time at microsecond granularity.
			 * \return The current time.
			 */
			virtual unsigned micros() = 0;
			/**
			 * \brief Blocks the current thread the specified number of microseconds.
			 * \param howLong The number of microseconds to block.
			 */
			virtual void delayMicroseconds(unsigned howLong) = 0;

			
			virtual unsigned millis() = 0;
			virtual void delayMilliseconds(unsigned howLong) = 0;
		};
	}
}
