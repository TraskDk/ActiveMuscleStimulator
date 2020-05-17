#include "logic/stimulator_app.h"
#include <cstdlib>
#include <stdexcept>

namespace ams
{
	namespace testing
	{
		int test_interrupts(int rate);
	}
}

//namespace ams
//{
//	void test_controller();
//}
//int tracking_test(const char* learning_file);
//int gyro_speed_test();

//ams::test_controller();
//if(argc>1)
//	return tracking_test(argv[1]);
//return gyro_speed_test();

int main(int argc, const char ** argv)
{
	//try
	//{
	//	ams::testing::test_interrupts(atoi(argv[1]));
	//}catch(std::exception & ex)
	//{
	//	printf("error: %s\n", ex.what());
	//}
	//return 0;
	
	auto app = new ams::logic::stimulator_app();
	app->run();
	delete app;
	

}