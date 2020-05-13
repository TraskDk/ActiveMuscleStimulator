#include "input_controller.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			bool input_controller::translate_event(const input_event& evt, int& button, int& state)
			{
				switch (evt.type)
				{
				case EV_KEY:
				case EV_ABS:
					button = evt.code;
					state = evt.value;
					return true;
				default:
					return false;
				}
			}
		}
	}
}
