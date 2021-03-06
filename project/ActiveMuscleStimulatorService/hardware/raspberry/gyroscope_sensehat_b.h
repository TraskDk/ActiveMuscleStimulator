#pragma once
#include "hardware/gyroscope.h"
#include "interrupt_handler.h"
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
			 * \brief Specifies the amount of low-pass filtering being applied.
			 */
			enum gyro_low_pass_filtering_value : byte
			{
				gyro_low_pass_filtering_disabled = 0x00,
				gyro_low_pass_filtering_noise_bandwidth_229_8 = 0x01,
				gyro_low_pass_filtering_noise_bandwidth_187_6 = 0x09,
				gyro_low_pass_filtering_noise_bandwidth_154_3 = 0x11,
				gyro_low_pass_filtering_noise_bandwidth_73_3 = 0x19,
				gyro_low_pass_filtering_noise_bandwidth_35_9 = 0x21,
				gyro_low_pass_filtering_noise_bandwidth_17_8 = 0x29,
				gyro_low_pass_filtering_noise_bandwidth_8_9 = 0x31,
				gyro_low_pass_filtering_noise_bandwidth_376_5 = 0x39
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
				 * \brief The target number of samples per second.
				 */
				float sampling_rate;

				/**
				 * \brief The amount of low-pass filtering being applied.
				 */
				gyro_low_pass_filtering_value low_pass_filtering;

				/**
				 * \brief Manage processing using interrupts.
				 */
				bool use_interrupts;
				
				/**
				 * \brief We fill in some default settings automatically.
				 */
				gyroscope_sensehat_b_settings()
				{
					// fill in all the default values here.
					gyro_scale = gyro_scale_500;
					accel_scale = accel_scale_4G;
					sampling_rate = 100;
					low_pass_filtering = gyro_low_pass_filtering_noise_bandwidth_35_9;
					use_interrupts = false;
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
				bool read(movement::movement_vector& vec) override;				
			protected:
				void write_byte(byte reg, byte val) { i2c_.write_byte(reg, val); }
				int read_byte(byte reg) { return i2c_.read_byte(reg); }
				void enable_irq(bool dataReadyEnable);
				bool is_data_ready();

				i2c_handler i2c_;
				float accel_scale_;
				float gyro_scale_;
				unsigned sample_micros_;
				
				unsigned last_sample_micros_;
				interrupt_handler* interrupt_handler_;
			};
		}
	}
}
