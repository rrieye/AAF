#include "cli.hpp"
#include <iostream>
#include <cctype>

// If a quote character (plain or typographic) starts at line[i], returns its
// plain form (" or ') and sets len to its size in bytes. Otherwise returns 0.
static char quote_at(const std::string& line, size_t i, size_t& len)
{
    if (line[i] == '"' || line[i] == '\'')
    {
        len = 1;
        return line[i];
    }

    if (line.compare(i, 3, "“") == 0 || line.compare(i, 3, "”") == 0)
    {
        len = 3;
        return '"';
    }

    if (line.compare(i, 3, "‘") == 0 || line.compare(i, 3, "’") == 0)
    {
        len = 3;
        return '\'';
    }

    return 0;
}

std::optional<std::string> read_command(std::istream& in) 
{
    std::string result;
    std::string line;
    char quote_type = 0;

    while (std::getline(in, line)) 
    {
        size_t i = 0;

        while (i < line.length()) 
        {
            size_t len = 1;
            char q = quote_at(line, i, len);

            if (q != 0 && quote_type == 0) 
            {
                quote_type = q;
                result += q;
            } 
            else if (q != 0 && q == quote_type) 
            {
                quote_type = 0;
                result += q;
            } 
            else if (q != 0) 
            {
                // another kind of quote inside a string, e.g. an apostrophe: keep it as is
                result += line.substr(i, len);
            } 
            else if (line[i] == ';' && quote_type == 0) 
            {
                return result; 
            } 
            else 
            {
                char c = line[i];

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

            i += len;
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
