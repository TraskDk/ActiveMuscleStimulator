#pragma once
#include "gyroscope.h"
#include "i2c_handler.h"

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			/**
			 * \brief Specifies the range to use when encoding accelerometer values.
			 */
			enum accel_scale_value : byte
			{
				accel_scale_2G = 0x00,
				accel_scale_4G = 0x02,
				accel_scale_8G = 0x04,
				accel_scale_16G = 0x06
			};

			/**
			 * \brief Specifies the range to use when encoding gyroscope values.
			 */
			enum gyro_scale_value : byte
			{
				gyro_scale_250 = 0x00,
				gyro_scale_500 = 0x02,
				gyro_scale_1000 = 0x04,
				gyro_scale_2000 = 0x06
			};
			
			/**
			 * \brief Structure for specifying settings to use for the gyroscope device.
			 */
			struct gyroscope_sensehat_b_settings
			{
				/**
				 * \brief The scaling to use for gyroscope values.
				 */
				gyro_scale_value gyro_scale;
				/**
				 * \brief The scaling to use for the accelerometer values.
				 */
				accel_scale_value accel_scale;
				
				/**
				 * \brief We fill in some default settings automatically.
				 */
				gyroscope_sensehat_b_settings()
				{
					// fill in all the default values here.
					gyro_scale = gyro_scale_500;
					accel_scale = accel_scale_4G;
				}
			};

			/**
			 * \brief The driver for the Waveshare Sense HAT (B) gyroscope.
			 */
			class gyroscope_sensehat_b final : public gyroscope
			{
			public:
				/**
				 * \brief Creates an instance of this driver using the specified settings.
				 * \param settings The settings to use for this instance.
				 */
				gyroscope_sensehat_b(gyroscope_sensehat_b_settings& settings);
				/**
				 * \brief Powering off the sensor hardware as part of the destructor.
				 */
				~gyroscope_sensehat_b() override;

				/**
				 * \brief Read the current movement data from the sensor.
				 * \param vec The target vector into which the data will be read.
				 * \return An indicator if data was read or not.
				 */
				bool read(math::movement_vector& vec) override;
				
			protected:
				void write_byte(byte reg, byte val) { i2c_.write_byte(reg, val); delay(1); }
				int read_byte(byte reg) { return i2c_.read_byte(reg); }
				void enable_irq(bool dataReadyEnable);
				bool is_data_ready();
			private:
				i2c_handler i2c_;
				float accel_scale_;
				float gyro_scale_;
			};
		}
	}
}
