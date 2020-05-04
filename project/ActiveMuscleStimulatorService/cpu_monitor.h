#pragma once
#include <sys/times.h>
#include <cstdio>

namespace ams
{
	namespace hardware
	{
		class cpu_monitor final
		{			
		public:
			cpu_monitor() { reset(); }
			void reset() { start_clock_ = times(&start_times_); }
			void print_usage() const
			{
				tms end_times = {};
				const auto end_clock = times(&end_times);
				const auto scale = 100.0f / static_cast<float>(end_clock - start_clock_);
				const auto user_percent = scale * static_cast<float>(end_times.tms_utime - start_times_.tms_utime);
				const auto system_percent = scale * static_cast<float>(end_times.tms_stime - start_times_.tms_stime);
				const auto other_percent = scale * static_cast<float>((end_times.tms_cutime - start_times_.tms_cutime) + (end_times.tms_cstime - start_times_.tms_cstime));
				const auto total_percent = user_percent + system_percent + other_percent;
				printf("CPU usage=%3.1f%% (user=%3.1f%%, system=%3.1f%%, other=%3.1f%%)\n", 
					total_percent, user_percent, system_percent, other_percent);
			}
		private:
			clock_t start_clock_;
			tms start_times_;
		};
	}
}