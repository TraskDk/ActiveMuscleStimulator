#include <wiringPi.h>
#include <stdexcept>
#include <cmath>
#include "gyroscope_sensehat_b.h"
#include "i2c_handler.h"
namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			//
			// Accelerometer and gyroscope exists in ICM20948.
			//
			//
			const byte ICM20948_ADDRESS = 0x68;

			// user bank 0
			const byte REG_ADD_WIA = 0x00;
			const byte REG_VAL_WIA = 0xea;

			const byte REG_ADD_PWR_MGMT_1 = 0x06;
			const byte REG_VAL_PWR_DEVICE_RESET = 0x80;
			const byte REG_VAL_PWR_SLEEP = 0x40;
			const byte REG_VAL_RUN_MODE = 0x01;
			const byte REG_VAL_RUN_DISABLE_TEMP = 0x08;
			
			const byte REG_ADD_INT_STATUS_1 = 0x1a;

			const byte REG_ADD_REG_BANK_SEL = 0x7F;
			const byte REG_VAL_REG_BANK_0 = 0x00;
			const byte REG_VAL_REG_BANK_1 = 0x10;
			const byte REG_VAL_REG_BANK_2 = 0x20;
			const byte REG_VAL_REG_BANK_3 = 0x30;

			const byte REG_ADD_ACCEL_XOUT = 0x2D;

			// user bank 1
			// user bank 2
			const byte REG_ADD_GYRO_SMPLRT_DIV = 0x00;
			const byte REG_ADD_GYRO_CONFIG_1 = 0x01;
			
			const byte REG_ADD_ACCEL_SMPLRT_DIV_2 = 0x11;
			
			const byte REG_ADD_ACCEL_CONFIG = 0x14;
			const byte REG_VAL_BIT_ACCEL_DLPF = 0x01;     /* bit[0]   */

			inline byte sample_rate_div(float rate)
			{
				if (rate > 1125) return 0;
				if (rate < 4.5) return 255;
				return static_cast<byte>(floorf((1125.0f / rate) - 1));
			}
			
			gyroscope_sensehat_b::gyroscope_sensehat_b(gyroscope_sensehat_b_settings& settings) : i2c_(ICM20948_ADDRESS)
			{
				/* - user bank 0 register */
				write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
				write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_DEVICE_RESET);
				delay(10);

				if (REG_VAL_WIA != read_byte(REG_ADD_WIA))
					throw std::runtime_error("Unable to initialize SenseHat.");

				const byte mode = REG_VAL_RUN_MODE | REG_VAL_RUN_DISABLE_TEMP;
				write_byte(REG_ADD_PWR_MGMT_1, mode);

				/* user bank 2 register */
				const byte gyro_config = settings.low_pass_filtering | settings.gyro_scale;
				const byte accel_config = settings.low_pass_filtering | settings.accel_scale;

				const byte sampling_rate_div = sample_rate_div(settings.sampling_rate);
				
				write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_2);
				write_byte(REG_ADD_GYRO_SMPLRT_DIV, sampling_rate_div);
				write_byte(REG_ADD_GYRO_CONFIG_1, gyro_config);
				write_byte(REG_ADD_ACCEL_SMPLRT_DIV_2, sampling_rate_div);
				write_byte(REG_ADD_ACCEL_CONFIG, accel_config);

				/* user bank 0 register */
				write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
				delay(100);

				switch (settings.accel_scale)
				{
				case accel_scale_2G: accel_scale_ = 2.0f / 32768.0f; break;
				case accel_scale_4G: accel_scale_ = 4.0f / 32768.0f; break;
				case accel_scale_8G: accel_scale_ = 8.0f / 32768.0f; break;
				case accel_scale_16G: accel_scale_ = 16.0f / 32768.0f; break;
				default: throw std::out_of_range("Unknown accelerometer scaling.");
				}

				switch (settings.gyro_scale)
				{
				case gyro_scale_250: gyro_scale_ = 250.0f / 32768.0f / 360.0f; break;
				case gyro_scale_500: gyro_scale_ = 500.0f / 32768.0f / 360.0f; break;
				case gyro_scale_1000: gyro_scale_ = 1000.0f / 32768.0f / 360.0f; break;
				case gyro_scale_2000: gyro_scale_ = 2000.0f / 32768.0f / 360.0f; break;
				default: throw std::out_of_range("Unknown gyroscope scaling.");
				}
			}

			gyroscope_sensehat_b::~gyroscope_sensehat_b()
			{
				try
				{
					// reset and put to sleep...
					write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
					write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_DEVICE_RESET);
					delay(10);
					write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_SLEEP);
				}
				catch(std::exception & ex)
				{
					/* ignore, we were just trying to clean up nicely in the destructor. */
				}
			}

			bool gyroscope_sensehat_b::is_data_ready()
			{
				return read_byte(REG_ADD_INT_STATUS_1) > 0;
			}

			inline float swap_and_scale(const byte * data, const float scale)
			{				
				const auto value = static_cast<short>(data[0] << 8 | data[1]);
				return static_cast<float>(value) * scale;
			}

			bool gyroscope_sensehat_b::read(movement::movement_vector& vec)
			{
				if (i2c_.read_byte(REG_ADD_INT_STATUS_1) == 0) return false;

				byte raw_data[12] = {};
				i2c_.read_range(REG_ADD_ACCEL_XOUT, raw_data, sizeof(raw_data));
				vec.values[0] = swap_and_scale(raw_data + 0, accel_scale_);
				vec.values[1] = swap_and_scale(raw_data + 2, accel_scale_);
				vec.values[2] = swap_and_scale(raw_data + 4, accel_scale_);
				vec.values[3] = swap_and_scale(raw_data + 6, gyro_scale_);
				vec.values[4] = swap_and_scale(raw_data + 8, gyro_scale_);
				vec.values[5] = swap_and_scale(raw_data + 10, gyro_scale_);
				return true;
			}


		}
	}
}