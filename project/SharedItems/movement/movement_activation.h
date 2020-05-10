#pragma once

#include "amstypes.h"

namespace ams
{
	namespace movement
	{
		struct movement_activation
		{
			static const int num_activation_channels = 2;
			int channels[num_activation_channels];
		};
	}
}