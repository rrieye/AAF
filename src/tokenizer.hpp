#pragma once
#include <string>
#include <vector>

enum TokenType
{
    token_ident,
    token_string,
    token_lparen,
    token_rparen,
    token_comma,
    token_equals,
    token_end
};

struct Token
{
    TokenType type;
    std::string text;
    size_t pos;
};

std::vector<Token> tokenize(const std::string& text);
