#include "tokenizer.hpp"
#include "command.hpp"
#include <cctype>

std::vector<Token> tokenize(const std::string& text)
{
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < text.length())
    {
        char c = text[i];

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            i++;
        }
        else if (std::isalpha(static_cast<unsigned char>(c)))
        {
            size_t start = i;
            while (i < text.length() && (std::isalnum(static_cast<unsigned char>(text[i])) || text[i] == '_'))
            {
                i++;
            }
            tokens.push_back({token_ident, text.substr(start, i - start), start});
        }
        else if (c == '"')
        {
            size_t start = i;
            std::string value;
            i++;

            while (i < text.length() && text[i] != '"')
            {
                value += text[i];
                i++;
            }

            if (i >= text.length())
            {
                throw ParseError("Syntax error at position " + std::to_string(start) + ": unclosed string");
            }

            i++;
            tokens.push_back({token_string, value, start});
        }
        else if (c == '(' || c == ')' || c == ',' || c == '=')
        {
            TokenType type = token_comma;
            if (c == '(')
            {
                type = token_lparen;
            }
            else if (c == ')')
            {
                type = token_rparen;
            }
            else if (c == '=')
            {
                type = token_equals;
            }
            tokens.push_back({type, std::string(1, c), i});
            i++;
        }
        else
        {
            throw ParseError("Syntax error at position " + std::to_string(i) + ": unexpected character '" + std::string(1, c) + "'");
        }
    }

    tokens.push_back({token_end, "", text.length()});
    return tokens;
}
