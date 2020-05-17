#pragma once

#include <pthread.h>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			class interrupt_handler
			{
			public:
				interrupt_handler(int pin);
				~interrupt_handler();
				bool wait(int wait_ms);
				int fd() const { return fd_; }
			private:
				const int pin_;
				int fd_;
			};
		}
	}
}