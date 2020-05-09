#pragma once

#include "smart_list.h"

namespace ams
{
	namespace collections
	{
		template<typename T, int block_size_bits = 3, int initial_capacity = 3>
		class small_list : public smart_list<T, block_size_bits, initial_capacity>
		{
		};
	}
}

