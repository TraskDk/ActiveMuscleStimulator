#pragma once
#include "movement_vector.h"

namespace ams
{
	namespace hardware
	{
		class gyroscope
		{
		public:
			virtual ~gyroscope() = default;
			virtual bool read(math::movement_vector& vec) = 0;
		};
	}
}
