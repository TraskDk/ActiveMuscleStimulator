#pragma once

namespace ams
{
	namespace testing
	{
		class tone_generator
		{
		private:
			const int _note_count;
			const int* const _notes;
			const int _note_time_ms;
			int _note_usecs[37]{};
		public:
			tone_generator(int note_count, const int* notes, int note_time_ms);
			bool get_state(int time, bool& state, int& usec);
		};
	}
}