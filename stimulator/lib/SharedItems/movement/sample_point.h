#pragma once

#include "amstypes.h"
#include "movement_vector.h"
#include "movement_activation.h"
#include "collections/collection.h"
#include <stdio.h>

namespace ams
{
	namespace movement
	{
		using namespace collections;

		struct sample_point
		{
			/**
			 * \brief The timestamp associated with this sample.
			 */
			uint64 time_us;
			movement_vector vec;
			movement_activation act;
			
			void write_to(FILE* output) const;
			bool read_from(FILE* input);
		};
		
		
	}
}
