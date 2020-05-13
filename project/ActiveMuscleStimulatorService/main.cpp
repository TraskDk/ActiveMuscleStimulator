#include "logic/stimulator_app.h"

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
	auto app = new ams::logic::stimulator_app();
	app->run();
	delete app;
	

}