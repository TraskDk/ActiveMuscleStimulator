#pragma once
#include "read_only_collection.h"

namespace ams
{
	namespace collections
	{
		/**
		 * \brief An abstraction over a collection of items.
		 * \tparam T The type of item in the collection.
		 */
		template<typename T>
		class collection : public read_only_collection<T>
		{
		public:
			/**
			 * \brief Returns a reference to the element at the specified index in the collection.
			 * \return A reference to an element.
			 */
			virtual T& operator [] (int idx) = 0;
			/**
			 * \brief Returns a reference to the first element in the collection.
			 * \return A reference to an element.
			 */
			virtual T& first() = 0;
			/**
			 * \brief Returns a reference to the last element in the collection.
			 * \return A reference to an element.
			 */
			virtual T& last() = 0;
			/**
			 * \brief Adds a reference to an added slot at the end of the collection.
			 * \return A reference to an element.
			 */
			virtual T& add() = 0;
			/**
			 * \brief Adds the specified element to the collection.
			 */
			virtual void add(const T& value) { add() = value; }
		};
	}
}