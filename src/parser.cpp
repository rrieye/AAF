#include "parser.hpp"
#include "tokenizer.hpp"
#include <cctype>

std::string to_upper(std::string s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        s[i] = std::toupper(static_cast<unsigned char>(s[i]));
    }
    return s;
}

bool is_keyword(const Token& token, const std::string& word)
{
    return token.type == token_ident && to_upper(token.text) == word;
}

bool is_reserved(const std::string& name)
{
    const char* reserved[] = {"CREATE", "INSERT", "INTO", "SELECT", "FROM", "FULL_JOIN", "ON", "WHERE", "INDEXED"};
    std::string up = to_upper(name);

    for (const char* word : reserved)
    {
        if (up == word)
        {
            return true;
        }
    }
    return false;
}

std::string describe(const Token& token)
{
    if (token.type == token_end)
    {
        return "end of command";
    }
    if (token.type == token_string)
    {
        return "string \"" + token.text + "\"";
    }
    return "'" + token.text + "'";
}

void syntax_error(const std::string& expected, const Token& got)
{
    throw ParseError("Syntax error at position " + std::to_string(got.pos) + ": expected " + expected + ", got " + describe(got));
}

void expect_symbol(const std::vector<Token>& tokens, size_t& pos, TokenType type, const std::string& what)
{
    if (tokens[pos].type != type)
    {
        syntax_error(what, tokens[pos]);
    }
    pos++;
}

void expect_keyword(const std::vector<Token>& tokens, size_t& pos, const std::string& word)
{
    if (!is_keyword(tokens[pos], word))
    {
        syntax_error("keyword " + word, tokens[pos]);
    }
    pos++;
}

std::string read_name(const std::vector<Token>& tokens, size_t& pos, const std::string& what)
{
    const Token& token = tokens[pos];

    if (token.type != token_ident)
    {
        syntax_error(what, token);
    }
    if (is_reserved(token.text))
    {
        throw ParseError("Syntax error at position " + std::to_string(token.pos) + ": '" + token.text + "' is a reserved word, expected " + what);
    }

    pos++;
    return token.text;
}

Create parse_create(const std::vector<Token>& tokens, size_t& pos)
{
    Create cmd;
    cmd.tableName = read_name(tokens, pos, "table name");
    expect_symbol(tokens, pos, token_lparen, "'(' after table name");

    while (true)
    {
        size_t name_pos = tokens[pos].pos;
        Column col;
        col.name = read_name(tokens, pos, "column name");

        for (size_t i = 0; i < cmd.columns.size(); i++)
        {
            if (cmd.columns[i].name == col.name)
            {
                throw ParseError("Syntax error at position " + std::to_string(name_pos) + ": duplicate column '" + col.name + "'");
            }
        }

        col.indexed = false;
        if (is_keyword(tokens[pos], "INDEXED"))
        {
            col.indexed = true;
            pos++;
        }
        cmd.columns.push_back(col);

        if (tokens[pos].type == token_comma)
        {
            pos++;
        }
        else
        {
            break;
        }
    }

    expect_symbol(tokens, pos, token_rparen, "',' or ')' after column");
    return cmd;
}

Insert parse_insert(const std::vector<Token>& tokens, size_t& pos)
{
    Insert cmd;

    if (is_keyword(tokens[pos], "INTO"))
    {
        pos++;
    }
    cmd.tableName = read_name(tokens, pos, "table name");
    expect_symbol(tokens, pos, token_lparen, "'(' after table name");

    while (true)
    {
        if (tokens[pos].type != token_string)
        {
            syntax_error("a value in quotes", tokens[pos]);
        }
        cmd.values.push_back(tokens[pos].text);
        pos++;

        if (tokens[pos].type == token_comma)
        {
            pos++;
        }
        else
        {
            break;
        }
    }

    expect_symbol(tokens, pos, token_rparen, "',' or ')' after value");
    return cmd;
}

Select parse_select(const std::vector<Token>& tokens, size_t& pos)
{
    Select cmd;
    expect_keyword(tokens, pos, "FROM");
    cmd.tableName = read_name(tokens, pos, "table name");

    if (is_keyword(tokens[pos], "FULL_JOIN"))
    {
        pos++;
        JoinClause join;
        join.table2 = read_name(tokens, pos, "table name after FULL_JOIN");
        expect_keyword(tokens, pos, "ON");
        join.leftColumn = read_name(tokens, pos, "column name after ON");
        expect_symbol(tokens, pos, token_equals, "'=' in ON condition");
        join.rightColumn = read_name(tokens, pos, "column name after '='");
        cmd.join = join;
    }

    if (is_keyword(tokens[pos], "WHERE"))
    {
        pos++;
        WhereClause where;
        where.column = read_name(tokens, pos, "column name after WHERE");
        expect_symbol(tokens, pos, token_equals, "'=' in WHERE condition");

        if (tokens[pos].type == token_string)
        {
            where.isColumnRhs = false;
            where.rhs = tokens[pos].text;
            pos++;
        }
        else
        {
            where.isColumnRhs = true;
            where.rhs = read_name(tokens, pos, "column name or value in quotes after '='");
        }
        cmd.where = where;
    }

    return cmd;
}

Command parse(const std::string& text)
{
    std::vector<Token> tokens = tokenize(text);
    size_t pos = 0;
    Command cmd;

    if (tokens[pos].type != token_ident)
    {
        syntax_error("a command (CREATE, INSERT or SELECT)", tokens[pos]);
    }

    std::string word = to_upper(tokens[pos].text);
    if (word == "CREATE")
    {
        pos++;
        cmd.type = cmd_create;
        cmd.create = parse_create(tokens, pos);
    }
    else if (word == "INSERT")
    {
        pos++;
        cmd.type = cmd_insert;
        cmd.insert = parse_insert(tokens, pos);
    }
    else if (word == "SELECT")
    {
        pos++;
        cmd.type = cmd_select;
        cmd.select = parse_select(tokens, pos);
    }
    else
    {
        throw ParseError("Syntax error at position " + std::to_string(tokens[pos].pos) + ": unknown command '" + tokens[pos].text + "'");
    }

    if (tokens[pos].type != token_end)
    {
        syntax_error("end of command", tokens[pos]);
    }
    return cmd;
}
