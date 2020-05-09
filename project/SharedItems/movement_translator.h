#pragma once
#include "sample_point.h"

namespace ams
{
	namespace movement
	{
		class movement_translator
		{
		public:
			virtual ~movement_translator() = default;
			virtual void reset() = 0;
			virtual bool observe(uint64 time, const movement_vector& movement, movement_activation& activation, float& progress, float& likelihood) = 0;
		};

		movement_translator* create_symmetric_movement_translator(const read_only_collection<sample_point>& input_points);
	}
}