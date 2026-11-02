#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "command.hpp"

struct DbError : std::runtime_error 
{ 
    using std::runtime_error::runtime_error; 
};

struct Table 
{
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<std::string>> rows;
};

struct Result 
{
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> rows;
};

class Database 
{
public:
    std::string create_table(const Create& cmd);
    std::string insert(const Insert& cmd);
    Result select(const Select& cmd);
private:
    std::map<std::string, Table> tables_;
};