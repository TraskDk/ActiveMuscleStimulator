#pragma once

#include <linux/input.h>
#include "input_event_device.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{

			class input_controller final : public input_event_device
			{
			protected:
				bool translate_event(const input_event& evt, input_key& button, int& state) override;
			};
		}
	}
}
