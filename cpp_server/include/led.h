#ifndef LED_H
#define LED_H

#include <iostream>
#include <cstdint>

class Led
{
    private:
        enum Color_
        {
            Led_red_ = 1,
            Led_green_ = 2,
            Led_blue_ = 3,
        };

        bool enabled_;
        Color_ color_;
        unsigned int rate_;

    public:
        typedef Color_ Color;
        static const Color Red = Led_red_;
        static const Color Green = Led_green_;
        static const Color Blue = Led_blue_;
        static const unsigned int MaxRate = 5;

        Led();

        void on();
        void off();
        bool setColor(Color color);
        bool setRate(unsigned int rate);

        bool isEnabled() const;
        Color getColor() const;
        std::string getState() const;
        unsigned int getRate() const;
};

std::ostream& operator<<(std::ostream& oss, const Led &led);
std::ostream& operator<<(std::ostream& oss, const Led::Color &color);

#endif // LED_H

