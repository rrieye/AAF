#include "cli.hpp"
#include <iostream>
#include <cctype>

void replace_smart_quotes(std::string& str) 
{
    std::string d_left = "“";
    std::string d_right = "”";
    std::string s_left = "‘";
    std::string s_right = "’";
    
    size_t pos = 0;
    
    while ((pos = str.find(d_left, pos)) != std::string::npos) 
    {
        str.replace(pos, d_left.length(), "\"");
    }
    
    pos = 0;
    
    while ((pos = str.find(d_right, pos)) != std::string::npos) 
    {
        str.replace(pos, d_right.length(), "\"");
    }

    pos = 0;
    
    while ((pos = str.find(s_left, pos)) != std::string::npos) 
    {
        str.replace(pos, s_left.length(), "'");
    }
    
    pos = 0;
    
    while ((pos = str.find(s_right, pos)) != std::string::npos) 
    {
        str.replace(pos, s_right.length(), "'");
    }
}

std::optional<std::string> read_command() 
{
    std::string result;
    std::string line;
    char quote_type = 0;

    while (std::getline(std::cin, line)) 
    {
        replace_smart_quotes(line);

        for (size_t i = 0; i < line.length(); ++i) 
        {
            char c = line[i];

            if ((c == '"' || c == '\'') && quote_type == 0) 
            {
                quote_type = c;
                result += c;
            } 
            else if (c == quote_type) 
            {
                quote_type = 0;
                result += c;
            } 
            else if (c == ';' && quote_type == 0) 
            {
                return result; 
            } 
            else 
            {
                if (quote_type == 0 && std::isspace(static_cast<unsigned char>(c))) 
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

        if (quote_type != 0) 
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