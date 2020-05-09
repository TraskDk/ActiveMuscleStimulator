#include "sample_point.h"
#include <stdexcept>

namespace ams
{
	namespace movement
	{
		void sample_point::write_to(FILE* output) const
		{
#ifdef _WIN32
			fprintf(output, "%I64d,%d,%d,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f\n",
#else
			fprintf(output, "%Ld,%d,%d,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f\n",
#endif
				time_us, act.channels[0], act.channels[1],
				vec.values[0], vec.values[1], vec.values[2],
				vec.values[3], vec.values[4], vec.values[5]);
		}

		bool sample_point::read_from(FILE* input)
		{
			if (input == nullptr) return false;
#ifdef _WIN32
			int res = fscanf_s(input, "%I64d,%d,%d,%f,%f,%f,%f,%f,%f\n",
#else
			int res = fscanf(input, "%Ld,%d,%d,%f,%f,%f,%f,%f,%f\n",
#endif
				& time_us, &act.channels[0], &act.channels[1],
				&vec.values[0], &vec.values[1], &vec.values[2],
				&vec.values[3], &vec.values[4], &vec.values[5]);
			return res == 9;
		}
		
		class simple_sample_point_log final : public sample_point_log
		{
		private:
			FILE* file_;
		public:
			simple_sample_point_log(const char * filename)
			{
				file_ = fopen(filename, "c");
				if (file_ == nullptr)
					throw std::runtime_error("unable to open file");
			}
			void append(const sample_point& point) override
			{
				point.write_to(file_);
			};
			~simple_sample_point_log()
			{
				fclose(file_);
			}
		};


		bool sample_point_log::read(const char* filename, collections::collection<sample_point>& points)
		{
			auto f = fopen(filename, "r");
			if (f == nullptr) return false;
			sample_point pt{};
			while (pt.read_from(f))
				points.add(pt);
			fclose(f);
			return true;
		}

		sample_point_log* sample_point_log::create(const char* filename)
		{
			return new simple_sample_point_log(filename);
		}

		
	}
}
