//
// Created by Tyler Bowers on 4/8/18.
//

#ifndef UAV_AUTOPILOT_I2C_H_
#define UAV_AUTOPILOT_I2C_H_

#include <stdio.h> 
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#define OK 0
#define ERROR -1

class I2cDevice
{
public:
    I2cDevice();

    int i2cInit(std::string device, uint8_t address);
    void i2cClose();
    
    // DEPRECATE
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    char readByte(uint8_t address, uint8_t subAddress);
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest);

    int i2cWrite(uint8_t data, size_t count, uint8_t address);
    int i2cRead(uint8_t *data, size_t count, uint8_t address);
    uint8_t i2cGetAddress()
    {
        return mAddress;
    }

private:
    int i2cOpen();
    int i2cSetAddress(uint8_t address);
    int i2cVerifyAddress(uint8_t address);

    std::string mDevice;
    int mFd;
    uint8_t mAddress;
};

#endif // UAV_AUTOPILOT_I2C_H_
