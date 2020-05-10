#include "inference.h"

namespace ams
{
	namespace movement
	{
		namespace inference
		{
			using namespace model;

			inline float get_error_contrib(const movement_vector& vec, const statistics_value& stats, int idx)
			{
				const auto e = (vec.values[idx] - stats.means[idx]) * stats.inv_std_devs[idx];
				return e * e;
			}

			float get_sample_error(const movement_vector& vec, const statistics_value& stats)
			{
				// todo: consider an implementation here that adapts to the size of the 
				return
					get_error_contrib(vec, stats, 0) +
					get_error_contrib(vec, stats, 1) +
					get_error_contrib(vec, stats, 2) +
					get_error_contrib(vec, stats, 3) +
					get_error_contrib(vec, stats, 4) +
					get_error_contrib(vec, stats, 5);
			}
		}
	}
}