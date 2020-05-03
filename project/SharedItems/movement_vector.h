#pragma once

#include "amstypes.h"

namespace ams
{
	namespace math
	{
		struct movement_vector
		{
			uint64 time;
			static const int num_movement_dimensions= 6;
			float values[num_movement_dimensions];
		};
	}
}
