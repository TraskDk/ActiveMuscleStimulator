#pragma once

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
		};

	}
}