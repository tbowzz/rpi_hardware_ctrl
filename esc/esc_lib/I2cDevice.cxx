//
// Created by Tyler Bowers on 4/8/18.
//

#include "I2cDevice.h"

I2cDevice::I2cDevice()
{

}

int I2cDevice::i2cInit(std::string device, uint8_t address)
{
	mDevice = device;
	mAddress = address;
	if (i2cOpen() != OK)
	{
		return ERROR;
	}
	if (i2cSetAddress(mAddress) != OK)
	{
		return ERROR;
	}

	return OK;
}

int I2cDevice::i2cOpen()
{
	mFd = open(mDevice.c_str(), O_RDWR);

	if (mFd < 0)
	{
		UTIL.log(ERROR_LOG, "I2C: Unable to open device %s", mDevice.c_str());
		return ERROR;
	}
	else
	{
		if (i2cSetAddress(mAddress) < 0)
		{
			return ERROR;
		}
		UTIL.log(INFO_LOG, "I2C: Connected to %s", mDevice.c_str());
		return OK;
	}
}

int I2cDevice::i2cSetAddress(uint8_t address)
{
	if (ioctl(mFd, I2C_SLAVE, address) < 0)
	{
		UTIL.log(ERROR_LOG, "I2C: Unable to set address to %#02x", address);
		i2cClose();
		return ERROR;
	}
	UTIL.log(DEBUG_LOG, "I2C: Address set to %#02x", address);
	return OK;
}

int I2cDevice::i2cVerifyAddress(uint8_t address)
{
	if (address != mAddress)
	{
		return ERROR;
	}
	return OK;
}

void I2cDevice::i2cClose()
{
	int ret = close(mFd);
	if (!ret)
	{
		UTIL.log(DEBUG_LOG, "I2C: Device closed.");
	}
}

int I2cDevice::i2cWrite(uint8_t data)
{
	int ret = write(mFd, &data, 1);
	if (ret < 0)
	{
		UTIL.log(ERROR_LOG, "I2C: Problem writing to %#02x. Write return value: %d", mAddress, ret);
		return ERROR;
	}
	UTIL.log(DEBUG_LOG, "I2C: Wrote %#02x to %#02x and return value was: %d", data, mAddress, ret);
	return OK;
}

int I2cDevice::i2cRead(uint8_t &data)
{
	int ret = read(mFd, &data, 1);
	if (ret < 0)
	{
		UTIL.log(ERROR_LOG, "I2C: Problem reading from %#02x. Read return value: %d", mAddress, ret);
		return ERROR;
	}
	UTIL.log(DEBUG_LOG, "I2C: Read from %#02x and return value was: %d and data = %d", mAddress, ret, data);
	return OK;
}
