#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#include "led.h"

class Command
{
    protected:
        std::string arg_;
        std::string response_;

    public:
        static const std::string OkResponse;
        static const std::string FailedResponse;

        Command(const std::string &arg);
        virtual ~Command();

        virtual void process(Led &led) = 0;
        std::string response() const;
};

class GetLedStateCommand : public Command
{
    public:
        GetLedStateCommand(const std::string &arg) : Command(arg) {}
        ~GetLedStateCommand() {}

        void process(Led &led);
};

class SetLedStateCommand : public Command
{
    public:
        SetLedStateCommand(const std::string &arg) : Command(arg) {}
        ~SetLedStateCommand() {}

        void process(Led &led);
};

class GetLedColorCommand : public Command
{
    public:
        GetLedColorCommand(const std::string &arg) : Command(arg) {}
        ~GetLedColorCommand() {}

        void process(Led &led);
};

class SetLedColorCommand : public Command
{
    public:
        SetLedColorCommand(const std::string &arg) : Command(arg) {}
        ~SetLedColorCommand() {}

        void process(Led &led);
};

class GetLedRateCommand : public Command
{
    public:
        GetLedRateCommand(const std::string &arg) : Command(arg) {}
        ~GetLedRateCommand() {}

        void process(Led &led);
};

class SetLedRateCommand : public Command
{
    public:
        SetLedRateCommand(const std::string &arg) : Command(arg) {}
        ~SetLedRateCommand() {}

        void process(Led &led);
};

#endif // COMMAND_H

