#pragma once

namespace AMS
{
	class composer
	{
	private:		
		const int _note_count;
		const int* const _notes;
		const int _note_time_ms;
		int _note_usecs[37];
	public:
		composer(int note_count, const int* const notes, const int note_time_ms);
		bool get_state(int time, bool& state, int& usec);
	};
}