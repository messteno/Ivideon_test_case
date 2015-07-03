#include <regex>

#include "logger.h"
#include "parser.h"

Parser::Parser(const std::string &query) :
    query_(query)
{
}

std::shared_ptr<Command> Parser::parse()
{
    std::size_t pos = query_.find(" ");

    if (pos != std::string::npos)
    {
        command_ = query_.substr(0, pos);
        arg_ = query_.substr(pos + 1);

        // trim spaces
        std::stringstream stream(arg_);
        stream >> arg_;
    }
    else
        command_ = query_;

    if (command_ == "get-led-state")
        return std::make_shared<GetLedStateCommand>(arg_);
    else if (command_ == "set-led-state")
        return std::make_shared<SetLedStateCommand>(arg_);
    else if (command_ == "get-led-color")
        return std::make_shared<GetLedColorCommand>(arg_);
    else if (command_ == "set-led-color")
        return std::make_shared<SetLedColorCommand>(arg_);
    else if (command_ == "get-led-rate")
        return std::make_shared<GetLedRateCommand>(arg_);
    else if (command_ == "set-led-rate")
        return std::make_shared<SetLedRateCommand>(arg_);

    return NULL;
}

