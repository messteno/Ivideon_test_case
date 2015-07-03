#include "led.h"

Led::Led() :
    enabled_(false),
    color_(Red),
    rate_(0)
{
}

void Led::on()
{
    enabled_ = true;
}

void Led::off()
{
    enabled_ = false;
}

bool Led::setColor(Color color)
{
    color_ = color;
    return true;
}

bool Led::setRate(unsigned int rate)
{
    if (rate > MaxRate)
        return false;

    rate_ = rate;
    return true;
}

bool Led::isEnabled() const
{
    return enabled_;
}

Led::Color Led::getColor() const
{
    return color_;
}

unsigned int Led::getRate() const
{
    return rate_;
}

std::string Led::getState() const
{
    if (enabled_ == true)
        return "on";
    return "off";
}

std::ostream& operator<<(std::ostream& oss, const Led::Color &color)
{
    switch (color)
    {
        case Led::Red:
            oss << "red";
            break;
        case Led::Green:
            oss << "green";
            break;
        case Led::Blue:
            oss << "blue";
            break;
    }
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const Led &led)
{
    oss << "["
        << led.getState() << "|" 
        << led.getColor() << "|"
        << led.getRate() << "]";
    return oss;
}

