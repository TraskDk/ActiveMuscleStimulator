#pragma once

namespace ams
{
	namespace hardware
	{
		class relay
		{
		public:
			virtual ~relay() = default;
			virtual void set(int channel, bool state) = 0;
		};
	}
}
