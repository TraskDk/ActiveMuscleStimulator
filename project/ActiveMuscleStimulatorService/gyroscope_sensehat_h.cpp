#include <unistd.h>
#include <wiringPi.h>
#include <stdexcept>
#include <cmath>
#include "gyroscope_sensehat_b.h"
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

			const byte REG_ADD_USER_CTRL = 0x03;
			const byte REG_VAL_BIT_I2C_MST_EN = 0x20;
			const byte REG_VAL_BIT_I2C_IF_DIS = 0x10;

			const byte REG_ADD_INT_ENABLE = 0x10;
			const byte REG_ADD_INT_ENABLE_1 = 0x11;
			const byte REG_ADD_INT_ENABLE_2 = 0x12;
			const byte REG_ADD_INT_ENABLE_3 = 0x13;
			const byte REG_ADD_INT_ENABLE_4 = 0x14;

			const byte REG_ADD_INT_STATUS = 0x19;
			const byte REG_ADD_INT_STATUS_1 = 0x1a;
			const byte REG_ADD_INT_STATUS_2 = 0x1b;
			const byte REG_ADD_INT_STATUS_3 = 0x1c;
			const byte REG_ADD_INT_STATUS_4 = 0x1d;

			
			const byte REG_ADD_REG_BANK_SEL = 0x7F;
			const byte REG_VAL_REG_BANK_0 = 0x00;
			const byte REG_VAL_REG_BANK_1 = 0x10;
			const byte REG_VAL_REG_BANK_2 = 0x20;
			const byte REG_VAL_REG_BANK_3 = 0x30;

			const byte REG_ADD_ACCEL_XOUT = 0x2D;
			const byte REG_ADD_ACCEL_YOUT = 0x2F;
			const byte REG_ADD_ACCEL_ZOUT = 0x31;
			const byte REG_ADD_GYRO_XOUT = 0x33;
			const byte REG_ADD_GYRO_YOUT = 0x35;
			const byte REG_ADD_GYRO_ZOUT = 0x37;

			// user bank 1
			// user bank 2
			const byte REG_ADD_GYRO_SMPLRT_DIV = 0x00;
			const byte REG_ADD_GYRO_CONFIG_1 = 0x01;

			const byte REG_VAL_BIT_DLPCFG_0 = 0x00;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_1 = 0x08;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_2 = 0x10;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_3 = 0x18;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_4 = 0x20;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_5 = 0x28;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_6 = 0x30;   /* bit[5:3] */
			const byte REG_VAL_BIT_DLPCFG_7 = 0x38;   /* bit[5:3] */

			const byte REG_VAL_BIT_GYRO_DLPF = 0x01;       /* bit[0]   */
			
			const byte REG_ADD_ACCEL_SMPLRT_DIV_2 = 0x11;
			
			const byte REG_ADD_ACCEL_CONFIG = 0x14;
			const byte REG_VAL_BIT_ACCEL_DLPF = 0x01;     /* bit[0]   */

			inline byte sample_rate_div(float rate)
			{
				if (rate > 1125) return 0;
				if (rate < 4.5) return 255;
				return static_cast<byte>(floorf((1125.0f / rate) - 1));
			}
			
			gyroscope_sensehat_b::gyroscope_sensehat_b(gyroscope_sensehat_b_settings& settings)
			{
				fd_ = wiringPiI2CSetup(ICM20948_ADDRESS);
				if (REG_VAL_WIA != read_byte(REG_ADD_WIA))
					throw std::runtime_error("Unable to initialize SenseHat.");
				/* reset and set power mode */
				
				/* - user bank 0 register */
				write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
				write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_DEVICE_RESET);
				delay(10);
				// todo: consider enabling SPI instead of I2C?
				write_byte(REG_ADD_USER_CTRL, REG_VAL_BIT_I2C_IF_DIS | REG_VAL_BIT_I2C_MST_EN);

				if (REG_VAL_WIA != read_byte(REG_ADD_WIA))
					throw std::runtime_error("Unable to initialize SenseHat.");

				const byte mode = REG_VAL_RUN_MODE | REG_VAL_RUN_DISABLE_TEMP;
				write_byte(REG_ADD_PWR_MGMT_1, mode);

				/* user bank 2 register */
				const byte gyro_config = REG_VAL_BIT_DLPCFG_2 | REG_VAL_BIT_GYRO_DLPF | settings.gyro_scale;
				const byte accel_config = REG_VAL_BIT_DLPCFG_2 | REG_VAL_BIT_ACCEL_DLPF | settings.accel_scale;

				//const byte sampling_rate_div = 0x07; // 0x07 is 140 Hz
				const byte sampling_rate_div = 0x0a; // 0x0a is 102 Hz.
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
				enable_irq(true);
			}

			gyroscope_sensehat_b::~gyroscope_sensehat_b()
			{
				//todo: is this the way to turn it off?
				write_byte(REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
				write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_DEVICE_RESET);
				delay(10);
				write_byte(REG_ADD_PWR_MGMT_1, REG_VAL_PWR_SLEEP);
				close(fd_);
			}

			void gyroscope_sensehat_b::enable_irq(bool dataReadyEnable)
			{
				write_byte(REG_ADD_INT_ENABLE_1, dataReadyEnable ? 1 : 0);
			}


			bool gyroscope_sensehat_b::is_data_ready()
			{
				return read_byte(REG_ADD_INT_STATUS_1) > 0;
			}

			bool gyroscope_sensehat_b::read(math::movement_vector& vec)
			{
				while (!is_data_ready())
					return false;
				auto ax = read_short(REG_ADD_ACCEL_XOUT);
				auto ay = read_short(REG_ADD_ACCEL_YOUT);
				auto az = read_short(REG_ADD_ACCEL_ZOUT);
				auto gx = read_short(REG_ADD_GYRO_XOUT);
				auto gy = read_short(REG_ADD_GYRO_YOUT);
				auto gz = read_short(REG_ADD_GYRO_ZOUT);
				vec.values[0] = static_cast<float>(ax) * accel_scale_;
				vec.values[1] = static_cast<float>(ay) * accel_scale_;
				vec.values[2] = static_cast<float>(az) * accel_scale_;
				vec.values[3] = static_cast<float>(gx) * gyro_scale_;
				vec.values[4] = static_cast<float>(gy) * gyro_scale_;
				vec.values[5] = static_cast<float>(gz) * gyro_scale_;
				
				return true;
			}
		}
	}
}