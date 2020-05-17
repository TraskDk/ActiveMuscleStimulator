#pragma once
#include <amstypes.h>

namespace ams
{
	namespace hardware
	{
		namespace raspberry
		{
			/**
			 * \brief Class that enables more efficient transfer of data to and from devices when using I2C.
			 */
			class i2c_handler
			{
			public:
				/**
				 * \brief Creates an instance of this class for communicating with the specified device.
				 * \param device The I2C device identifier.
				 */
				i2c_handler(byte device);

				~i2c_handler();

				/**
				 * \brief Writes a byte to the specified device register.
				 * \param reg The register at which to write the byte.
				 * \param value The byte to write.
				 */
				void write_byte(byte reg, byte value);
				/**
				 * \brief Reads the byte at the specified device register.
				 * \param reg The device register from which to read the byte.
				 * \return The byte currently stored at the specified register.
				 */
				byte read_byte(byte reg);
				/**
				 * \brief Reads a number of bytes from the devices starting from the specified register.
				 * \param reg The register from which to start reading.
				 * \param buffer The destination buffer into which data will be read.
				 * \param count The number of register bytes from which to read into the buffer.
				 * \return Return value is currently unspecified.
				 */
				int read_range(byte reg, void* buffer, ushort count);
			private:
				byte device_;
				int fd_;
			};
		}
	}

}