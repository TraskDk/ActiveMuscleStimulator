#pragma once
namespace ams
{
	namespace hardware
	{
		/**
		 * \brief Abstract class that represents an input device that can be used for controlling the behavior of the software.
		 */
		class input_device
		{
		public:
			/**
			 * \brief Can be used to check if an input device is currently available.
			 */
			virtual bool is_available() = 0;
			/**
			 * \brief Checks if an input event is available. If returning true, the button and state fields have been populated with information aboud the event.
			 * \param button Set to the button identifier if an event was read.
			 * \param state Set to the button state on if an event was read.
			 * \param block Indicates if this should be a blocking call, in which case the call will not return immediately when no data is available.
			 */
			virtual bool get_event(int& button, int& state, const bool block = false) = 0;
			~input_device() = default;
		};
	}
}
