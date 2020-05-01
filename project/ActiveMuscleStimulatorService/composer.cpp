#include "composer.h"
#include <math.h>
namespace AMS
{
	composer::composer(int note_count, const int * const notes, const int note_time_ms) :
		_note_count(note_count), _notes(notes), _note_time_ms(note_time_ms)
	{
		for(auto i=0;i<37;i++)
			_note_usecs[i] = static_cast<int>(1000000.0 / (30.0 * pow(2, i / 12.0)));
	}

	bool composer::get_state(int time, bool& state, int& usec)
	{
		auto note_index = static_cast<int>(time / (1000 * _note_time_ms));
		if (note_index >= _note_count) return false;

		auto note = _notes[note_index];
		auto note_offset = time - (1000 * note_index * _note_time_ms);
		if (note >= 0)
		{
			auto note_phase_len = _note_usecs[note];
			auto note_half_phase_len = note_phase_len / 2;
			auto phase = note_offset % note_phase_len;
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