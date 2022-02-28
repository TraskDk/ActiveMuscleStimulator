#pragma once
#include <functional>

namespace ams
{
	namespace collections
	{
		/**
		 * \brief An abstraction over a read-only collection of items.
		 * \tparam T The type of item in the collection.
		 */
		template<typename T>
		class read_only_collection {
		public:
			virtual ~read_only_collection() = default;
			/**
			 * \brief Returns a reference to the first element in the collection.
			 * \return A reference to an element.
			 */
			const T& first() const { return (*this)[0]; }
			/**
			 * \brief Returns a reference to the last element in the collection.
			 * \return A reference to an element.
			 */
			const T& last() const { return (*this)[length() - 1]; }
			/**
			 * \brief Returns the number of elements in the collection.
			 * \return Element count.
			 */
			virtual int length() const = 0;
			/**
			 * \brief Returns a reference to the element at the specified index in the collection.
			 * \return A reference to an element.
			 */
			virtual const T& operator[] (int idx) const = 0;
			/**
			 * \brief Counts the number of elements for which the supplied test is true.
			 * \return The number of maching elements.
			 */
			int count(std::function<bool(const T &)> test) const
			{
				int res = 0;
				int cnt = length();
				for(int i=0;i<cnt;i++)
					if(test((*this)[i]))
						res++;
				return res;
			}
			/**
			 * \brief Checks if any of the elements satisfies the specified test.
			 * \return Indicator if any such element exists.
			 */
			bool any(std::function<bool(const T &)> test) const
			{
				int cnt = length();
				for(int i=0;i<cnt;i++)
					if(test((*this)[i]))
						return true;
				return false;
			}	
			/**
			 * \brief Finds the index of the first element satisfying the specified test.
			 * \return The index of the first match or -1 otherwise.
			 */
			int index_of(std::function<bool(const T &)> test, int start_index=0) const
			{
				int cnt = length();
				for(int i=start_index;i<cnt;i++)
					if(test((*this)[i]))
						return i;
				return -1;
			}
		};

	}
}