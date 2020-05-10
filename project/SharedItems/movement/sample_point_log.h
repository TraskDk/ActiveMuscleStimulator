#pragma once

#include "sample_point.h"
namespace ams
{
	namespace movement
	{
		class sample_point_log
		{
		public:
			virtual void append(const sample_point& point) = 0;
			virtual ~sample_point_log() = default;

			static bool read(const char* filename, collection<sample_point>& points);
			static sample_point_log* create(const char* filename);
		};
	}
}