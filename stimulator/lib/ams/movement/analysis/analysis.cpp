#include "analysis.h"
#include "movement/sample_point.h"
#include <stdio.h>

namespace ams
{
	namespace movement
	{
		namespace analysis
		{
			void make_groups(const read_only_collection<sample_point>& points, int channel, small_list<small_list<sample_point>>& res)
			{
				// first sample:
				// 1582228117778524,0,0,0.14,-0.96,-0.28,-0.05,0.05,-0.03
				auto state = 0;

				// add a dummy list to append to
				for (auto i = 0; i < points.length(); i++)
				{
					const auto& pt = points[i];
					const auto act = pt.act.channels[channel];
					switch (state)
					{
					case 0:
						if (act > 0) { res.add().add(pt); state = 1; }
						break;
					case 1:
						res.last().add(pt);
						if (act == 0) state = 2;
						break;
					case 2:
						res.last().add(pt);
						if (act == 1) { res.add().add(pt); state = 1; }
						break;
					}
				}
			}
		}
	}
}