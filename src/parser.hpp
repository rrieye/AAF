#pragma once
#include <string>
#include "command.hpp"

enum CommandType
{
    cmd_create,
    cmd_insert,
    cmd_select
};

struct Command
{
    CommandType type;
    Create create;
    Insert insert;
    Select select;
};

Command parse(const std::string& text);
