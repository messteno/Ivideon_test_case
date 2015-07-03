#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>

#include "command.h"

class Parser
{
    private:
        const std::string &query_;
        std::string command_;
        std::string arg_;

    public:
        Parser(const std::string &query);
        std::shared_ptr<Command> parse();
};

#endif // PARSER_H

