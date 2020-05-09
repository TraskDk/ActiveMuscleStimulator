#pragma once

#include "amstypes.h"
#include "movement_vector.h"
#include "movement_activation.h"
#include "collection.h"
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
		
		class sample_point_log
		{
		public:
			virtual void append(const sample_point& point)=0;
			virtual ~sample_point_log() = default;

			static bool read(const char* filename, collection<sample_point>& points);
			static sample_point_log * create(const char* filename);
		};
		
	}
}
