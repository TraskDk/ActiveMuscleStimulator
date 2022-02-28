#include "sample_point.h"
#include <stdexcept>

namespace ams
{
	namespace movement
	{
		void sample_point::write_to(FILE* output) const
		{
			//#ifdef _WIN32
			//const int r = fprintf(output, "%I64d,%d,%d,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f\n",
			//#else
			const int r = fprintf(output, "%Ld,%d,%d,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f\n",
			//#endif
				time_us, act.channels[0], act.channels[1],
				vec.values[0], vec.values[1], vec.values[2],
				vec.values[3], vec.values[4], vec.values[5]);
			if (r < 0) throw std::runtime_error("Unable to write sample point to file.");
		}

		bool sample_point::read_from(FILE* input)
		{
			if (input == nullptr) return false;
			//#ifdef _WIN32
			//int res = fscanf_s(input, "%I64d,%d,%d,%f,%f,%f,%f,%f,%f\n",
			//#else
			int res = fscanf(input, "%Ld,%d,%d,%f,%f,%f,%f,%f,%f\n",
			//#endif
				& time_us, &act.channels[0], &act.channels[1],
				&vec.values[0], &vec.values[1], &vec.values[2],
				&vec.values[3], &vec.values[4], &vec.values[5]);
			return res == 9;
		}
				
	}
}
