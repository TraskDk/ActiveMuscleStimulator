#pragma once
#include "hardware/relay.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			/**
			 * \brief Relay driver for Relay 4 Zero - 4 Channel 3V Relay Board for Raspberry Pi from SB Components Ltd
			 */
			class relay_sbz4 : public relay
			{
			public:
				/**
				 * \brief Creates an instance of this relay driver.
				 */
				relay_sbz4();
				virtual ~relay_sbz4();
				/**
				 * \brief Resets the relay by setting all channels to false state.
				 */
				void reset();
				/**
				 * \brief Specifies if the relay on the specified channel should be enabled or not.
				 * \param channel The relay channel in question.
				 * \param state The state we want the relay to be in.
				 */
				virtual void set(int channel, bool state);
			private:
				bool current_states_[4];
			};
		}
	}
}