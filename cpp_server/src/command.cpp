#include "logger.h"
#include "command.h"

const std::string Command::OkResponse = "OK";
const std::string Command::FailedResponse = "FAILED";

Command::Command(const std::string &arg) :
    arg_(arg)
{
}

Command::~Command()
{
}

std::string Command::response() const
{
    return response_;
}

void GetLedStateCommand::process(Led &led)
{
    if (!arg_.empty())
        response_ = FailedResponse;
    else
        response_ = OkResponse + " " + led.getState();
}

void SetLedStateCommand::process(Led &led)
{
    if (arg_ == "on")
    {
        led.on();
        response_ = OkResponse;
    }
    else if (arg_ == "off")
    {
        led.off();
        response_ = OkResponse;
    }
    else
    {
        response_ = FailedResponse;
    }
}

void GetLedColorCommand::process(Led &led)
{
    if (!arg_.empty())
        response_ = FailedResponse;
    else
    {
        std::stringstream ss;
        ss << OkResponse << " " << led.getColor();
        response_ = ss.str();
    }
}

void SetLedColorCommand::process(Led &led)
{
    if (arg_ == "red")
    {
        led.setColor(Led::Red);
        response_ = OkResponse;
    }
    else if (arg_ == "green")
    {
        led.setColor(Led::Green);
        response_ = OkResponse;
    }
    else if (arg_ == "blue")
    {
        led.setColor(Led::Blue);
        response_ = OkResponse;
    }
    else
    {
        response_ = FailedResponse;
    }
}

void GetLedRateCommand::process(Led &led)
{
    if (!arg_.empty())
        response_ = FailedResponse;
    else
    {
        std::stringstream ss;
        ss << OkResponse << " " << led.getRate();
        response_ = ss.str();
    }
}

void SetLedRateCommand::process(Led &led)
{
    if (arg_.empty())
        response_ = FailedResponse;
    else
    {
        std::stringstream ss(arg_);
        unsigned int rate;
        ss >> rate;

        if (led.setRate(rate))
            response_ = OkResponse;
        else
            response_ = FailedResponse;
    }
}

