#ifndef SPI_R_H_INCLUDED
#define SPI_R_H_INCLUDED

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

// Raspberry pi has 3 or 4 ports for i2c
// i2c-0 (gpios 27 & 28) is used by firmware
// this can be changed via config.txt but check documentation
// i2c-1 uses gpio 2 (SDA) and 3 (SCL) this is the default
// there are also ports used by HDMI
// make sure to enable i2c in raspi-config
//
// This class uses very basic file access.
// For anything more sophisticated use SMB
//
// connections are
// MOSI  19(GPIO10)  data in
// MISO  21(GPIO9)   data out
// SCLK  23(GPIO11)  clock
// CE0   24(GPIO8)   chip select spidev0.0
// CE1   26(GPIO7)   chip select spidev0.1

namespace rw
{

class SpiBase
// Base class is probably uneccessary since
// there can be only one device but it fits in
// with my convention
{
public:
    SpiBase(const char* port = "/dev/spidev0.0")
    {
        handle = open(port,O_RDWR);
        if(handle < 0)
            throw std::runtime_error("Failed to open spi port");
    }
    ~SpiBase(){close(handle);}

    int gethandle(){return handle;}

private:
    int handle;
};

class Spi
{
public:
    Spi(SpiBase& base):base(base)
    {
        // All transfers set to 8 bit
        // Not sure what speeds are good but this works
        transfer.bits_per_word = 8;
        transfer.speed_hz = 1000000;
        handle = base.gethandle();
    }

    int sndrcv(uint8_t* snd, uint8_t* rcv, unsigned count)
    {
        // SPI always sends and receives count bytes
        // the two are overlapped not sequential
        // if snd or rcv is not needed pass nullptr
        transfer.tx_buf = (__u64)snd;
        transfer.rx_buf = (__u64)rcv;
        transfer.len = count;

        return ioctl(handle, SPI_IOC_MESSAGE(1), &transfer);
    }

private:
    SpiBase base;
    int handle;
    spi_ioc_transfer transfer = {0,0,0,0,0,0,0,0,0,0,0};
};

}


#endif // SPI_R_H_INCLUDED
