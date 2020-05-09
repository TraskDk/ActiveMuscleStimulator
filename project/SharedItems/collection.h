#pragma once
#include "read_only_collection.h"

namespace ams
{
	namespace collections
	{
		template<typename T>
		class collection : public read_only_collection<T>
		{
		public:
			virtual T& operator [] (int idx) = 0;
			virtual T& first() = 0;
			virtual T& last() = 0;
			virtual T& add() = 0;
			virtual void add(const T& value) { add() = value; }
		};
	}
}