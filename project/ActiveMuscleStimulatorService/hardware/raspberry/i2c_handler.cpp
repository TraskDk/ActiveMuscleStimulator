#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "i2c_handler.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			i2c_handler::i2c_handler(byte device)
			{
				fd_ = open("/dev/i2c-1", O_RDWR);
				if (fd_ < 0)
					throw std::runtime_error("Unable to open i2c device");
				device_ = device;
			}

			i2c_handler::~i2c_handler()
			{
				close(fd_);
			}


			void i2c_handler::write_byte(byte reg, byte value)
			{
				byte register_and_value[2] = { reg, value };
				i2c_msg msg{ .addr = device_, .flags = 0, .len = sizeof(register_and_value), .buf = register_and_value };
				i2c_rdwr_ioctl_data data{ .msgs = &msg, .nmsgs = 1 };
				const auto res = ioctl(fd_, I2C_RDWR, &data);
				if (res < 0)
					throw std::runtime_error("Unable to write byte via i2c");
			}
			
			byte i2c_handler::read_byte(byte reg)
			{
				byte value = 0;
				i2c_msg msg[2]
				{
					{.addr = device_, .flags = 0, .len = 1, .buf = &reg },
					{.addr = device_, .flags = I2C_M_RD, .len = 1 , .buf = &value }
				};
				i2c_rdwr_ioctl_data data{ .msgs = msg, .nmsgs = 2 };
				const auto res = ioctl(fd_, I2C_RDWR, &data);
				if (res < 0)
					throw std::runtime_error("Unable to read byte via i2c");
				return value;
			}

			int i2c_handler::read_range(byte reg, void * buffer, ushort count)
			{
				auto bytes = static_cast<unsigned char*>(buffer);
				i2c_msg msg[2]
				{
					{.addr = device_, .flags = 0, .len = 1, .buf = &reg },
					{.addr = device_, .flags = I2C_M_RD, .len = count , .buf = bytes}
				};
				i2c_rdwr_ioctl_data data{ .msgs = msg, .nmsgs = 2 };
				const auto res = ioctl(fd_, I2C_RDWR, &data);
				if (res < 0)
					throw std::runtime_error("Unable to read byte range via i2c");
				return res;
			}
			
		}
	}
}
