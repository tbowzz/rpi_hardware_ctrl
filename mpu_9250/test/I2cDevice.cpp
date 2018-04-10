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
        printf("I2C: Unable to open device %s", mDevice);
        return ERROR;
    }
    else
    {
        if (i2cSetAddress(mAddress) < 0)
        {
            return ERROR;
        }
        printf("I2C: Connected to %s", mDevice);
        return OK;
    }
}

int I2cDevice::i2cSetAddress(uint8_t address)
{
    if (ioctl(mFd, I2C_SLAVE, address) < 0)
    {
        printf("I2C: Unable to set address.");
        i2cClose();
        return ERROR;
    }
    printf("I2C: Address set to ...");
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
        printf("I2C: Device closed.");
    }
}

int I2cDevice::i2cWrite(uint8_t data, size_t count, uint8_t address)
{
    char data_write[2];
    data_write[0] = address;
    data_write[1] = data;
    int ret = write(mFd, data_write, count);
    if (ret < 0)
    {
        printf("I2C: Problem writing to address. Write return value: %s", std::to_string(ret));
        return ERROR;
    }
    printf("I2C: Wrote to device and return value was: %s", std::to_string(ret));
    return OK;
}

int I2cDevice::i2cRead(uint8_t *data, size_t count, uint8_t address)
{
    // char data_write[2];
    // data_write[0] = address;
    // data_write[1] = data;
    int ret = read(mFd, data, count);
    if (ret < 0)
    {
        printf("I2C: Problem reading from 0xTODO. Read return value: %s", std::to_string(ret));
        return ERROR;
    }
    printf("I2C: Read from device and return value was: %s", std::to_string(ret));
    return OK;
}

void I2cDevice::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
   char data_write[2];
   data_write[0] = subAddress;
   data_write[1] = data;
//    i2c.write(mAddress, data_write, 2, 0);
}

char I2cDevice::readByte(uint8_t address, uint8_t subAddress)
{
    char data[1]; // `data` will store the register data     
    char data_write[1];
    data_write[0] = subAddress;
    // i2c.write(mAddress, data_write, 1, 1); // no stop
    // i2c.read(mAddress, data, 1, 0); 
    return data[0]; 
}

void I2cDevice::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest)
{     
    char data[14];
    char data_write[1];
    data_write[0] = subAddress;
    // i2c.write(mAddress, data_write, 1, 1); // no stop
    // i2c.read(mAddress, data, count, 0); 
    for(int ii = 0; ii < count; ii++)
    {
        dest[ii] = data[ii];
    }

    // unsigned char buffer[60] = {0};
    // //----- READ BYTES -----
	// int length = count;			//<<< Number of bytes to read
	// if (read(m_i2c_fd, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	// {
	// 	//ERROR HANDLING: i2c transaction failed
	// 	printf("I2C: Failed to read from the i2c bus.");
	// }
	// else
	// {
    //     char str[128];
    //     sprintf(str, "%s", buffer);
	// 	printf("I2C: Data read: " + str);
	// }

    // dest = buffer;
} 
