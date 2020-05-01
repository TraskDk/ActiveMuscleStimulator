#include "relay.h"
#include <wiringPi.h>
#include <stdio.h>
#include "composer.h"

using namespace AMS;

const int note_count = 16;
const int notes0[note_count] = { 24, 24, 31, 31, 33, 33, 31, -1, 29, 29, 28, 28, 26, 26, 24, -1 };
const int notes1[note_count] = { 12,  0,  4,  4,  5,  5,  4, -1,  9,  7, 12,  4,  2,  7,  0, -1 };
const int notes2[note_count] = { 24, 24, 16, 16, 17, 17, 16, -1, 12, 11, 12, 12, 12, 11, 12, -1 };
const int notes3[note_count] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

inline int min(int a, int b)
{
	return a < b ? a : b;
}

int main(int argc, const char ** argv)
{
	if (wiringPiSetup() == -1)
	{
		printf("Unable to setup wiringPi.\n");
		return -1;
	}
	relay r;
	composer c0(note_count, notes0, 600);
	composer c1(note_count, notes1, 600);
	composer c2(note_count, notes2, 600);
	composer c3(note_count, notes3, 600);

	auto start_time = static_cast<int>(micros());
	
	while (true)
	{
		const auto time = micros() - start_time;
		
		bool state;
		int delay0, delay1, delay2, delay3;
		if (!c0.get_state(time, state, delay0)) break;
		r.set(0, state);
		if (!c1.get_state(time, state, delay1)) break;
		r.set(1, state);
		if (!c2.get_state(time, state, delay2)) break;
		r.set(2, state);
		if (!c3.get_state(time, state, delay3)) break;
		r.set(3, state);

		const auto delay = min(min(delay0, delay1), min(delay2, delay3));
		delayMicroseconds(delay);
	}
	return 0;
}