#ifndef I2C_R_H_INCLUDED
#define I2C_R_H_INCLUDED

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// Raspberry pi has 3 or 4 ports for i2c
// i2c-0 (gpios 27 & 28) is used by firmware
// this can be changed via config.txt but check documentation
// i2c-1 uses gpio 2 (SDA) and 3 (SCL) this is the default
// there are also ports used by HDMI
// make sure to enable i2c in raspi-config
//
// This class uses very basic file access.
// For anything more sophisticated use SMB

namespace rw
{

class I2cBase
{
public:
    I2cBase(const char* port = "/dev/i2c-1")
    {
        handle = open(port,O_RDWR);
        if(handle < 0)
            throw std::runtime_error("Failed to open i2c port");
    }
    ~I2cBase(){close(handle);}

    int check(uint8_t add)
    {
        // the same handle (file descriptor) may be used
        // to connect to multiple devices. This checks
        // that the correct address is targetted
        if(add != address)
        {
            if(ioctl(handle,I2C_SLAVE,add) < 0)
                throw std::runtime_error("i2c connect failed");
            else
                address = add;
        }
        return handle;
    }
private:
    int handle;
    uint8_t address = 0x00;
};

class I2c
{
public:
    I2c(I2cBase& base,uint8_t address):base(base),address(address){}

    void send(const uint8_t* buffer,int count)
    {
        write(base.check(address),buffer,count);
    }
    void receive(uint8_t* buffer,int count)
    {
        read(base.check(address),buffer,count);
    }
private:
    I2cBase base;
    uint8_t address;
};

}

#endif // I2C-R_H_INCLUDED
