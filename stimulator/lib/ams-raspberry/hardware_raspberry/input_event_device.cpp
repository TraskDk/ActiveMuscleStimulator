#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <poll.h>
#include <wiringPi.h>
#include <cerrno>
#include "input_event_device.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			input_event_device::input_event_device()
			{
				fd_ = -1;
			}

			bool input_event_device::is_available()
			{
				return open();
			}

			bool input_event_device::get_event(input_key& button, int& state, const bool block)
			{
				button = key_unknown;
				state = 0;
				if (!open())
				{
					if (block)
						delay(max_block_ms);
					return false;
				}
			retry:
				if (block) wait_event(fd_);
				input_event event = {};
				auto* evt = &event;
				if (!read_event(fd_, evt))
				{
					close();
					return false;
				}
				if (evt == nullptr)
					return false;

				if (!translate_event(*evt, button, state))
					goto retry;
				
				return true;
			}
			input_event_device::~input_event_device()
			{
				close();
			}
				

			bool input_event_device::open()
			{
				if (fd_ != -1) return true;
				const auto now = millis();
				const auto elapsed = static_cast<unsigned>(now - last_open_attempt_ms_);
				if (elapsed < device_check_interval_ms)
					return false;
				last_open_attempt_ms_ = now;
				fd_ = ::open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
				return fd_ != -1;
			}
			
			void input_event_device::close()
			{
				if (fd_ == -1) return;
				::close(fd_);
				fd_ = -1;
			}

			void input_event_device::wait_event(const int fd)
			{
				pollfd fds[1]{ {.fd = fd,	.events = POLLIN } };
				poll(fds, 1, max_block_ms);
			}

			bool input_event_device::read_event(const int fd, input_event*& evt)
			{
				const auto read_res = read(fd, evt, sizeof(input_event));
				if (read_res == sizeof(input_event))
					return true;
				if (read_res < 0 && errno == EAGAIN)
				{
					evt = nullptr;
					return true;
				}
				return false;
			}
			int last_open_attempt_ms_;
			int fd_;
		};
	}
}
