#pragma once

namespace ams
{
	namespace movement
	{
		namespace symmetric
		{
			namespace model
			{
				struct statistics_value
				{
					static const int num_dimensions = movement_vector::num_movement_dimensions;
					float means[num_dimensions];
					float inv_std_devs[num_dimensions];
					statistics_value() : means{0}, inv_std_devs{0} {}
				};

				struct speed_statistics
				{
					float sequences_per_second_min;
					float sequences_per_second_avg;
					float sequences_per_second_max;
					speed_statistics() : sequences_per_second_min(0), sequences_per_second_avg(0), sequences_per_second_max(0) {}
				};

				struct activation_statistics {
					float enable_at_progress;
					float disable_at_progress;
					activation_statistics() : enable_at_progress(0), disable_at_progress(0) {}
				};

				template<const int num_progress_values>
				struct pattern_model
				{
					speed_statistics speed_stats;
					activation_statistics activation_stats;
					statistics_value values[num_progress_values];
					pattern_model() = default;
				};
			}
		}
	}
}
