#include "I2cDevice.h"

int main(int argc, char **argv)
{
	I2cDevice i2cDevice;
	if (i2cDevice.i2cInit("/dev/i2c-1", 0x40) != OK)
	{
		return ERROR;
	}
	return OK;
}
