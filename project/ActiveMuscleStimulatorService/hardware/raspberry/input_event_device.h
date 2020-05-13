#pragma once
#include "hardware/input_device.h"

struct input_event;

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			/**
			 * \brief Encapsulates an input device that can be read by accessing /dev/input. This class manages variable availability of the input device. 
			 */
			class input_event_device : public input_device
			{
			public:
				input_event_device();
				bool is_available() override;
				bool get_event(int& button, int& state, bool block = false) override;
				virtual ~input_event_device();
			protected:
				/**
				 * \brief This method is used for translating an input event into a valid button and state.
				 * \param evt The input event to process.
				 * \param button The output button value to extract from the event.
				 * \param state  The output state value to extract from the event.
				 * \return true if an only if button and state could be extracted from the event.
				 */
				virtual bool translate_event(const input_event& evt, int& button, int& state) = 0;

			private:
				static const unsigned device_check_interval_ms = 1000;
				static const unsigned max_block_ms = 500;

				bool open();
				void close();
				static void wait_event(int fd);
				static bool read_event(int fd, input_event*& evt);

				int last_open_attempt_ms_;
				int fd_;
			};
		}
	}
}
