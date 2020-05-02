#include <wiringPi.h>
#include <cstdio>
#include "music.h"
#include "relay_sbz4.h"
#include "timing_wired.h"

using namespace ams;
using namespace testing;

int main(int argc, const char ** argv)
{
	if (wiringPiSetup() == -1)
	{
		printf("Unable to setup wiringPi.\n");
		return -1;
	}
	hardware::timing_wired timing;
	hardware::relay_sbz4 relay;	
	music::play(&timing, &relay);
	return 0;
}