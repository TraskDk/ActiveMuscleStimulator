#pragma once

namespace ams
{
	namespace collections
	{
		template<typename T>
		class read_only_collection {
		public:
			virtual ~read_only_collection() = default;			
			const T& first() const { return (*this)[0]; }
			const T& last() const { return (*this)[length() - 1]; }
			virtual int length() const = 0;
			virtual const T& operator[] (int idx) const = 0;
		};

	}
}