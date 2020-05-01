#pragma once

namespace AMS
{
	class relay
	{
	private:
		bool _current_states[4];
	public:
		relay();
		void reset();
		void set(const int channel, const bool state);
	};
}
