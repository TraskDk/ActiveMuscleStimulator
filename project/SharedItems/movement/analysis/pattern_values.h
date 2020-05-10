#pragma once

#include "movement/sample_point.h"

namespace ams
{
	namespace movement
	{
		namespace analysis
		{
			template<int num_progress_values>
			struct pattern_values
			{
				float pattern_duration_seconds; // time difference between first and last point.
				float activation_duration_seconds; // time until activation stopped.
				sample_point pattern_points[num_progress_values];
			};
		}
	}
}