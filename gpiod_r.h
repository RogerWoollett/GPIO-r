// gpiod_r.h
#ifndef GPIOD_R_H_INCLUDED
#define GPIOD_R_H_INCLUDED

// Use following commands to install prerequisites and build
// sudo apt install gpiod
// sudo apt install libgpiod-dev libgpiod-doc
// the docs are at /usr/share/doc/libgpiod-dev/html/index.html
// link with gpiodcxx

#include <gpiod.hpp>

namespace rw
{
enum pullups{up,down,off};

class GpioBase
// represents the chip controlling GPIOs
{
public:
    GpioBase(const char* chip = "gpiochip0"):chip(chip){}

    //get control of a line (gpio number)
    gpiod::line get_line(unsigned line){return chip.get_line(line);}

private:
    gpiod::chip chip;
};

class Gpio
// represents one gpio pin
{
public:
    Gpio(GpioBase & base,unsigned number):base(base),line(base.get_line(number))
    {
        //default to input
        line.request({"gpio-r",gpiod::line_request::DIRECTION_INPUT, 0});
    }
    void set_output(bool high)
    {
        //set output state and initially high or low
        line.set_direction_output(high);
    }
    void set_input()
    {
        line.set_direction_input();
    }
    bool read()
    {
        //get current state (high or low)
        return line.get_value();
    }
    void set(bool high)
    {
        //set state high or low
        line.set_value(high);
    }
    void pullup(pullups state)
    // set pullup state (initially off)
    {
        switch(state)
        {
        case up:
            line.set_flags(gpiod::line_request::FLAG_BIAS_PULL_UP);
            break;
        case down:
            line.set_flags(gpiod::line_request::FLAG_BIAS_PULL_DOWN);
            break;
        case off:
            line.set_flags(gpiod::line_request::FLAG_BIAS_DISABLE);
            break;
        }
    }

private:
    GpioBase base;
    gpiod::line line;
};

}
#endif // GPIOD-R_H_INCLUDED
