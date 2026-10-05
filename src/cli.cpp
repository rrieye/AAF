#include "cli.hpp"
#include <iostream>
#include <cctype>

void replace_smart_quotes(std::string& str) 
{
    std::string left = "“";
    std::string right = "”";
    size_t pos = 0;
    
    while ((pos = str.find(left, pos)) != std::string::npos) 
    {
        str.replace(pos, left.length(), "\"");
    }
    
    pos = 0;
    
    while ((pos = str.find(right, pos)) != std::string::npos) 
    {
        str.replace(pos, right.length(), "\"");
    }
}

std::optional<std::string> read_command() 
{
    std::string result;
    std::string line;
    bool in_quotes = false;

    while (std::getline(std::cin, line)) 
    {
        replace_smart_quotes(line);

        for (size_t i = 0; i < line.length(); ++i) 
        {
            char c = line[i];

            if (c == '"') 
            {
                in_quotes = !in_quotes;
                result += c;
            } 
            else if (c == ';' && !in_quotes) 
            {
                return result; 
            } 
            else 
            {
                if (!in_quotes && std::isspace(static_cast<unsigned char>(c))) 
                {
                    if (result.empty() || result.back() != ' ') 
                    {
                        result += ' ';
                    }
                } 
                else 
                {
                    result += c;
                }
            }
        }

        if (in_quotes) 
        {
            std::cout << "Syntax error: Unclosed quote detected.\n";
            return ""; 
        }

        if (!result.empty() && result.back() != ' ') 
        {
            result += ' ';
        }
    }

    if (!result.empty()) 
    {
        std::cout << "Syntax error: Missing ';' at the end of command.\n";
        return "";
    }
    
    return std::nullopt; 
}