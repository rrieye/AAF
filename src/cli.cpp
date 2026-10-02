#include "cli.hpp"
#include <iostream>
#include <cctype>

std::string read_command() 
{
    std::string command;
    bool in_quotes = false;
    char c;
    bool last_was_space = true;

    while (std::cin.get(c)) 
    {
        if (c == '"') 
        {
            in_quotes = !in_quotes;
            command += c;
            last_was_space = false;
        } 
        else if (!in_quotes && c == ';') 
        {
            std::string dummy;
            std::getline(std::cin, dummy);
            break; 
        } 
        else if (!in_quotes && std::isspace(static_cast<unsigned char>(c))) 
        {
            if (!last_was_space) 
            {
                command += ' ';
                last_was_space = true;
            }
        } 
        else 
        {
            command += c;
            last_was_space = false;
        }
    }

    if (!command.empty() && command.back() == ' ') 
    {
        command.pop_back();
    }

    return command;
}