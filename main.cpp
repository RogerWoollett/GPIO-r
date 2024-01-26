#include <iostream>
#include <thread>
#include <chrono>
#include "gpiod_r.h"
#include "i2c_r.h"
#include "spi_r.h"
#include "mcp3008.h"
#include "sht30.h"

// testing configuration
// red led on gpio 12
// yellow led on gpio 16
// button that when pressed grounds gpio 21
// i2c is used to access a SHT30 sensor
// SPI is used to access a mcp3008 a to d chip


int main()
{
    using namespace std::chrono_literals;

    // base objects
    rw::GpioBase gpio;
    rw::I2cBase i2c;
    rw::SpiBase spi;

    // gpios
    rw::Gpio red(gpio,12);
    rw::Gpio yellow(gpio,16);
    rw::Gpio button(gpio,21);

    red.set_output(false);
    yellow.set_output(true);
    button.pullup(rw::up);

    // i2c test
    SHT30 sht30(i2c);
    SHT30::Data data;
    sht30.get_data(data);
    std::cout << "Temperature " << data.temperature << "C Humidity " << data.humidity << "%\n";

    // spi
    MCP3008 adc(spi);

    int count = 10;
    while( count--)
    {
        if(button.read())
            std::cout << "up    ";
        else
            std::cout << "down  ";

        std::cout << adc.readvolts(0) << "volts\n";
        std::this_thread::sleep_for(1s);

        red.set(!red.read());
        yellow.set(!yellow.read());
    }

    red.set(false);
    yellow.set(false);

    return 0;
}
