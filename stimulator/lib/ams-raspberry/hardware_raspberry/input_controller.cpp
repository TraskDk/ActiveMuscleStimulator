#include "input_controller.h"
//#include <cstdio>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			bool input_controller::translate_event(const input_event& evt, input_key& button, int& state)
			{
				switch (evt.type)
				{
				case EV_KEY:
				case EV_ABS:
					//printf("KEY: %d %d\n", evt.code, evt.value);
					switch (evt.code)
					{
					case 297:
					case 313: // 8BitDo
						button = key_start;
						break;
					case 296:
					case 312: // 8BitDo
						button = key_select;
						break;
					case 292:
					case 308: // 8BitDo
						button = key_l;
						break;
					case 294:
					case 309: // 8BitDo
						button = key_r;
						break;
					case 289:
					case 305: // 8BitDo
						button = key_a;
						break;
					case 290:
					case 304: // 8BitDo
						button = key_b;
						break;
					case 288:
					case 307: // 8BitDo
						button = key_x;
						break;
					case 291:						
					case 306: // 8BitDo
						button = key_y;
						break;
					case 0:
						button = key_horizontal;
						break;
					case 1:
						button = key_vertical;
						break;
					default: button = key_unknown;
						state = 0;
						return false;
					}
					state = evt.value;
					return true;
				default:
					return false;
				}
			}
		}
	}
}
