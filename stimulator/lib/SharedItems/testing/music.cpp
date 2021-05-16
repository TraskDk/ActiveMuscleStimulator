#include "music.h"
#include "tone_generator.h"

namespace ams
{
	namespace testing
	{
		const static int note_duration_ms = 327;
		const static int note_count = 32;
		const static int notes0[note_count] = { 24, -1,  0, -1, -1, -1, -1, -1, 33, -1, 33, -1, 31, 31, -1, -1, 29, -1, 29, -1, 28, -1, 28, -1, 26, -1, 26, -1, 24, 24, 24, -1 };
		const static int notes1[note_count] = { -1, -1, 24, -1,  4, -1,  4, -1,  5, -1,  5, -1,  4,  4, -1, -1,  9, -1,  7, -1, 12, -1,  4, -1,  2, -1,  7, -1,  0,  0,  0, -1 };
		const static int notes2[note_count] = { -1, -1, -1, -1, 31, -1, 16, -1, 17, -1, 17, -1, 16, 16, -1, -1, 12, -1, 11, -1, 12, -1, 12, -1, 12, -1, 11, -1, 19, 19, 19, -1 };
		const static int notes3[note_count] = { -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1,  9, -1,  7, -1, 12, -1,  4, -1,  2, -1,  7, -1,  0,  0,  0, -1 };

		inline int min(int a, int b)
		{
			return a < b ? a : b;
		}

		void music::play(hardware::timing* const timing, hardware::relay* const r)
		{
			tone_generator c0(note_count, notes0, note_duration_ms);
			tone_generator c1(note_count, notes1, note_duration_ms);
			tone_generator c2(note_count, notes2, note_duration_ms);
			tone_generator c3(note_count, notes3, note_duration_ms);

			const auto start_time = static_cast<int>(timing->micros());

			while (true)
			{
				const auto time = timing->micros() - start_time;

				bool state;
				int delay0, delay1, delay2, delay3;
				if (!c0.get_state(time, state, delay0)) break;
				r->set(0, state);
				if (!c1.get_state(time, state, delay1)) break;
				r->set(1, state);
				if (!c2.get_state(time, state, delay2)) break;
				r->set(2, state);
				if (!c3.get_state(time, state, delay3)) break;
				r->set(3, state);

				const auto delay = min(min(delay0, delay1), min(delay2, delay3));
				while (true)
				{
					const auto elapsed = (timing->micros() - start_time) - time;
					const auto remaining_delay = delay - static_cast<int>(elapsed);
					if (remaining_delay <= 0) break;
					timing->delayMicroseconds(remaining_delay);
				}
			}
		}
	}
}
