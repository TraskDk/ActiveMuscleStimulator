#pragma once
#include "gyroscope.h"

namespace ams
{
	namespace hardware
	{
		class gyroscope_smoothing : public gyroscope
		{
		private:
			const float alpha_;
			gyroscope* const gyro_;
			float mean_values_[movement::movement_vector::num_movement_dimensions];
			bool first_;
		public:
			gyroscope_smoothing(const float alpha, gyroscope * gyro) : alpha_(alpha), gyro_(gyro), mean_values_{}, first_(true) { }
			
			bool read(movement::movement_vector& vec) override
			{
				if (!gyro_->read(vec)) return false;
				if(first_)
				{
					// copy initial values as mean values
					first_ = false;
					for (auto i = 0; i < movement::movement_vector::num_movement_dimensions; i++)
						mean_values_[i] = vec.values[i];
				}
				for(auto i=0;i< movement::movement_vector::num_movement_dimensions;i++)
				{
					const auto v = vec.values[i];
					const auto mean = mean_values_[i];
					vec.values[i] = v - mean;
					mean_values_[i] = (1 - alpha_) * mean + alpha_ * v;
				}
				return true;
			}

		};
	}
}
