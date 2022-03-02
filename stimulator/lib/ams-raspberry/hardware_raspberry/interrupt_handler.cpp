#include "interrupt_handler.h"
#include <pthread.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <poll.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>
#include <cstring>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			static bool file_exists(const char* filename)
			{
				struct stat buffer {};
				const auto exist = ::stat(filename, &buffer);
				return exist == 0;
			}

			static bool file_write(const char * filename, const char * content)
			{
				const auto fd = open(filename, O_WRONLY);
				if (fd < 0)
					return false;
				const auto status = write(fd, content, strlen(content));
				close(fd);
				return status >= 0;
			}

			static void unprepare(int pin)
			{
				char buffer[80];
				sprintf(buffer, "%d", pin);
				file_write("/sys/class/gpio/unexport", buffer);
			}
			
			static void prepare(int pin, int & fd)
			{
				char buffer[80];
				sprintf(buffer, "/sys/class/gpio/gpio%d/value", pin);
				if (file_exists(buffer))
					unprepare(pin);
				
				sprintf(buffer, "%d", pin);
				if (!file_write("/sys/class/gpio/export", buffer))
					throw std::runtime_error("Unable to export gpio pin.");

				sprintf(buffer, "/sys/class/gpio/gpio%d/direction", pin);
				if (!file_write(buffer, "in"))
					throw std::runtime_error("Unable to specify gpio pin direction.");

				sprintf(buffer, "/sys/class/gpio/gpio%d/edge", pin);
				if (!file_write(buffer, "rising"))
					throw std::runtime_error("Unable to specify gpio pin edge.");

				sprintf(buffer, "/sys/class/gpio/gpio%d/value", pin);
				fd = open(buffer, O_RDONLY);
				if(fd<0)
					throw std::runtime_error("Unable to open gpio pin value file.");
			}
			
			interrupt_handler::interrupt_handler(int pin) : pin_(pin), fd_(-1)
			{
				prepare(pin_, fd_);
			}

			bool interrupt_handler::wait(const int wait_ms)
			{
				pollfd pfd{ .fd = fd_, .events = POLLPRI }; //  | 
				lseek(fd_, 0, SEEK_SET);
				const auto poll_res = poll(&pfd, 1, wait_ms);
				if (poll_res < 0)
					throw std::runtime_error("Unable to perform poll on interrupt handle.");
				if (poll_res == 0) return false;
				char buf;
				const auto read_res = read(fd_, &buf, 1);
				if (read_res <= 0)
					throw std::runtime_error("Unable to perform blocking poll on interrupt handle.");
				return true;
			}
			
			interrupt_handler::~interrupt_handler()
			{
				if (fd_ > 0)
				{
					close(fd_);
					fd_ = -1;
					unprepare(pin_);
				}
			}
			
		}
	}
}
