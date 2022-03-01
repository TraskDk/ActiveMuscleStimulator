#include "sample_point_log.h"
#include <stdexcept>

namespace ams
{
	namespace movement
	{
		class simple_sample_point_log final : public sample_point_log
		{
		private:
			FILE* file_;
		public:
			simple_sample_point_log(const char* filename)
			{
				file_ = fopen(filename, "w");
				if (file_ == nullptr)
					throw std::runtime_error("unable to open file");
			}
			void append(const sample_point& point) override
			{
				point.write_to(file_);
			};
			~simple_sample_point_log()
			{
				fflush(file_);
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