#include "movement_translator.h"
#include "smart_list.h"
#include <cstdio>
#include <time.h>

class stopwatch
{
public:
	stopwatch() :clk_{}
	{
		reset();
	}
	void reset()
	{
		clk_= clock();
	}
	float seconds()
	{
		auto now = clock();
		return (now - clk_) * (1.0f / CLOCKS_PER_SEC);
	}
private:
	clock_t clk_;
};


using namespace ams;
using namespace movement;

const char* learning_file = R"(..\..\data\training_data.csv)";
const char* real_file = R"(..\..\data\real_data.csv)";
int main()
{	
	try
	{
		movement_translator* translator = nullptr;
		{
			printf("Learning from training file.\n");
			smart_list<sample_point> points;
			sample_point_log::read(learning_file, points);
			translator = create_symmetric_movement_translator(points);
		}

		{
			printf("Testing on real file.\n");

			auto num_good_points = 0;
			
			translator->reset();
			smart_list<sample_point> points;
			sample_point_log::read(real_file, points);
			
			auto likelihood = 0.0f;
			auto progress = 0.0f;
			movement_activation activation = {};

			auto start_time = points[0].time_us;
			stopwatch sw;
			for (auto i = 0; i < points.length(); i++)
			{
				auto& p = points[i];
				auto status = translator->observe(p.time_us, p.vec, activation, progress, likelihood);
				/* evaluate performance of model here */
				//auto seconds = (p.time_us - start_time) / 1000000.0f;
				if(status)
				{
					//printf("%4.2f\t%4.2f\n", seconds, progress);
					num_good_points++;
				}
				else
				{
					//printf("%4.2f\t\n", seconds);
				}
			}
			auto processing_seconds = sw.seconds();
			auto rate = static_cast<long>(points.length() / processing_seconds);
			printf("- processed %d vectors in %5.3f seconds (%d vectors/sec)\n", points.length(), processing_seconds, rate);

			auto coverage = 100.0f * num_good_points / points.length();
			printf("- reported good tracking on %d out of %d points (%3.1f%%).\n", num_good_points, points.length(), coverage);
			
		}
		printf("Done.\n");
	}
	catch(const std::exception & err)
	{
		printf("Error: %s", err.what());
		return 1;
	}
	return 0;
}
