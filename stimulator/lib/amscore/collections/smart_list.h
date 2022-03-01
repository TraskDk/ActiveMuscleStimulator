#pragma once

#include <stdexcept>
#include "collection.h"

namespace ams
{
	namespace collections
	{
		template<typename T, int block_size_bits = 8, int initial_capacity = 8>
		class smart_list : public collection<T>
		{
			struct smart_list_block { T elements_[1 << block_size_bits]; };
			
			typedef smart_list_block* smart_list_block_ptr;
			
		private:
			int count_;
			int block_count_;
			int block_capacity_;
			smart_list_block_ptr* blocks_;

			void extend_block_capacity()
			{
				// extend block capacity.
				const auto new_block_capacity = 2 * block_capacity_;
				const auto new_blocks = new smart_list_block_ptr[new_block_capacity]{ nullptr };
				for (auto i = 0; i < block_count_; i++)
					new_blocks[i] = blocks_[i];
				const auto temp = blocks_;
				blocks_ = new_blocks;
				block_capacity_ = new_block_capacity;
				delete[] temp;
			}
		public:
			smart_list()
			{
				count_ = 0;
				block_count_ = 0;
				block_capacity_ = initial_capacity;
				blocks_ = new smart_list_block_ptr[block_capacity_]{ nullptr };
			}
			~smart_list()
			{
				for (auto i = 0; i < block_count_; i++)
					delete blocks_[i];
				delete[] blocks_;
			}
			smart_list(const smart_list&) = delete;

			int length() const override
			{
				return count_;
			}

			T& operator [] (const int idx) override
			{
				if (idx < 0 || idx >= count_) throw std::out_of_range("invalid index");
				const auto block_num = idx >> block_size_bits;
				const auto block_index = idx & ((1 << block_size_bits) - 1);
				return blocks_[block_num]->elements_[block_index];
			}

			const T& operator [] (const int idx) const override
			{
				if (idx < 0 || idx >= count_) throw std::out_of_range("invalid index");
				const auto block_num = idx >> block_size_bits;
				const auto block_index = idx & ((1 << block_size_bits) - 1);
				return blocks_[block_num]->elements_[block_index];
			}

			T& first() override
			{
				return (*this)[0];
			}

			T& last() override
			{
				return (*this)[length() - 1];
			}

			T& add() override
			{
				const auto block_num = count_ >> block_size_bits;
				const auto block_index = count_ & ((1 << block_size_bits) - 1);

				if (block_num >= block_count_)
				{
					// extend with new block
					if (block_num >= block_capacity_)
						extend_block_capacity();
					blocks_[block_count_++] = new smart_list_block();
				}

				smart_list_block* const target_block = blocks_[block_num];
				++count_;
				return target_block->elements_[block_index];
			}

			void add(const T& value) override { add() = value; }

		};
	}
}
