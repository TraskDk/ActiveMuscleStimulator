#pragma once
#include <amstypes.h>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			class i2c_handler
			{
			public:
				i2c_handler(byte device);
				void write_byte(byte reg, byte value);
				byte read_byte(byte reg);
				int read_range(byte reg, void* buffer, ushort count);
			private:
				byte device_;
				int fd_;
			};
		}
	}

}