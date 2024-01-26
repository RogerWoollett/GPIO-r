#ifndef SHT30_H_INCLUDED
#define SHT30_H_INCLUDED

#include "i2c_r.h"

class SHT30
// class to access a SHT30 temperature/humidity sensor
{
public:
    SHT30(rw::I2cBase& base,uint8_t address = 0x44):i2c(base,address){}
    struct Data
    {
        float temperature;
        float humidity;
    };
    void get_data(Data& data)
    {
        i2c.send(cmd,sizeof(cmd));
        uint8_t buf[6];
        i2c.receive(buf,sizeof(buf));
        data.temperature = -45.0 +  175 * (buf[0] << 8 | buf[1])/0xffff;
        data.humidity = 100.0 * (buf[3] << 8 | buf[4])/0xffff;
    }
private:
    rw::I2c i2c;
    const uint8_t cmd[2] = {0x2c,0x06};
};


#endif // SHT30_H_INCLUDED
