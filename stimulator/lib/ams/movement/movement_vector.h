#pragma once

#include "amstypes.h"

namespace ams
{
	namespace movement
	{
		/**
		 * \brief A structure containing a vector of values from accelerometer.
		 */
		struct movement_vector
		{
			/**
			 * \brief The number of dimensions stored in movement vectors.
			 */
			static const int num_movement_dimensions = 6;
			/**
			 * \brief The values extracted from an accelerometer at the specified time.
			 */
			float values[num_movement_dimensions];
		};
	}
}
