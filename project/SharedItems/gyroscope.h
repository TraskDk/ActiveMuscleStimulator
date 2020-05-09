#pragma once
#include "movement_vector.h"

namespace ams
{
	namespace hardware
	{
		/**
		 * \brief Abstract class for reading movement vectors.
		 */
		class gyroscope
		{
		public:
			virtual ~gyroscope() = default;
			/**
			 * \brief Read the current movement data from the sensor.
			 * \param vec The target vector into which the data will be read.
			 * \return An indicator if data was read or not.
			 */
			virtual bool read(movement::movement_vector& vec) = 0;
		};
	}
}
