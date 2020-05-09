#pragma once

namespace ams
{
	namespace movement
	{
		namespace symmetric
		{
			namespace model
			{
				template<const int num_progress_values, const int num_speeds>
				struct model_likelihoods
				{
					float likelihoods[num_progress_values][num_speeds];
				};
			}
		}
	}
}
