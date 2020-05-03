#pragma once

namespace ams
{
	namespace hardware
	{
		/**
		 * \brief Abstract class for switching relays.
		 */
		class relay
		{
		public:
			virtual ~relay() = default;
			/**
			 * \brief Specifies if the relay on the specified channel should be enabled or not.
			 * \param channel The relay channel in question.
			 * \param state The state we want the relay to be in.
			 */
			virtual void set(int channel, bool state) = 0;
		};
	}
}
