#pragma once
#include <wiringPiI2C.h>
#include "gyroscope.h"
#include <wiringPiSPI.h>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			enum accel_scale_value : byte
			{
				accel_scale_2G = 0x00,
				accel_scale_4G = 0x02,
				accel_scale_8G = 0x04,
				accel_scale_16G = 0x06
			};

			enum gyro_scale_value : byte
			{
				gyro_scale_250 = 0x00,
				gyro_scale_500 = 0x02,
				gyro_scale_1000 = 0x04,
				gyro_scale_2000 = 0x06
			};
			
			struct gyroscope_sensehat_b_settings
			{
				gyro_scale_value gyro_scale;
				accel_scale_value accel_scale;
				
				gyroscope_sensehat_b_settings()
				{
					gyro_scale = gyro_scale_500;
					accel_scale = accel_scale_4G;
					// fill in default values here.
				}
			};
			
			class gyroscope_sensehat_b final : public gyroscope
			{
			public:
				gyroscope_sensehat_b(gyroscope_sensehat_b_settings& settings);
				~gyroscope_sensehat_b();
				virtual bool read(math::movement_vector& vec);
			protected:	
				void write_byte(byte reg, byte val) { wiringPiI2CWriteReg8(fd_, reg, val); }
				int read_byte(byte reg) { return wiringPiI2CReadReg8(fd_, reg); }
				int read_short(byte reg) { return static_cast<short>(read_byte(reg) << 8 | read_byte(static_cast<byte>(reg + 1))); }
				void enable_irq(bool dataReadyEnable);
				bool is_data_ready();
			private:
				int fd_;
				float accel_scale_;
				float gyro_scale_;
			};
		}
	}
}
