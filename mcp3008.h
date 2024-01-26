#ifndef MCP3008_H_INCLUDED
#define MCP3008_H_INCLUDED

// these are used by error checking
#include <errno.h>
#include <string.h>

// harware connections
// MOSI  19(GPIO10)  11 - data in
// MISO  21(GPIO9)   12 - data out
// SCLK  23(GPIO11)  13 - clock
// CE0   24(GPIO8)   10 - chip select spidev0.0
// CE1   26(GPIO7)   10 - chip select spidev0.1

class MCP3008
{
public:
    MCP3008(rw::SpiBase& base):spi(base){vref = 3.3;}

    int read(int channel)
    {
        uint8_t snd[3],rcv[3] = {0,0,0};
        snd[0] = 1;
        snd[1] = (0b1000 + channel) << 4;
        snd[2] = 0;
        int err = spi.sndrcv(snd,rcv,3);

        if(err < 0)
            std::cout << strerror(errno) << "\n";

        return rcv[1] << 8 | rcv[2];
    }

    float readvolts(int channel)
    {
        return vref*read(channel)/1024;
    }

private:
    rw::Spi spi;
    float vref;
};

#endif // MCP3008_H_INCLUDED
