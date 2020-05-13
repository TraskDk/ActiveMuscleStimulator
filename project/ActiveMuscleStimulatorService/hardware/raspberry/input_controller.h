#pragma once

#include <linux/input.h>
#include "input_event_device.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			enum controller_key
			{
				key_start = 297,
				key_select = 296,
				key_l = 292,
				key_r = 294,
				key_a = 289,
				key_b = 290,
				key_x = 288,
				key_y = 291,
				key_horizontal,
				key_vertical,
			};

			class input_controller final : public input_event_device
			{
			protected:
				bool translate_event(const input_event& evt, int& button, int& state) override;
			};
		}
	}
}
