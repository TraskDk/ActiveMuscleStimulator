#include <cmath>
#include "tone_generator.h"
namespace ams
{
	namespace testing
	{
		tone_generator::tone_generator(int note_count, const int* const notes, const int note_time_ms) :
			_note_count(note_count), _notes(notes), _note_time_ms(note_time_ms)
		{
			for (auto i = 0; i < 37; i++)
				_note_usecs[i] = static_cast<int>(1000000.0 / (32.7032 * pow(2, i / 12.0)));
		}

		bool tone_generator::get_state(int time, bool& state, int& usec)
		{
			const auto note_index = static_cast<int>(time / (1000 * _note_time_ms));
			if (note_index >= _note_count) return false;

			const auto note = _notes[note_index];
			const auto note_offset = time - (1000 * note_index * _note_time_ms);
			if (note >= 0)
			{
				const auto note_phase_len = _note_usecs[note];
				const auto note_half_phase_len = note_phase_len / 2;
				const auto phase = note_offset % note_phase_len;
				if (phase < note_half_phase_len) {
					state = true;
					usec = note_half_phase_len - phase;
				}
				else {
					state = false;
					usec = note_phase_len - phase;
				}
			}
			else
			{
				state = false;
				usec = 1000 * _note_time_ms - note_offset;
			}
			return true;
		}
	}
}
