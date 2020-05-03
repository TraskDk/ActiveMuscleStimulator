#pragma once
#include "relay.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			class relay_sbz4 : public relay
			{
			public:
				relay_sbz4();
				virtual ~relay_sbz4();
				void reset();
				virtual void set(const int channel, const bool state);
			private:
				bool current_states_[4];
			};
		}
	}
}