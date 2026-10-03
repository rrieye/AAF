#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Column {
    std::string name;
    bool indexed = false;
};

struct Create {
    std::string tableName;
    std::vector<Column> columns;
};

struct Insert {
    std::string tableName;
    std::vector<std::string> values;
};

struct JoinClause {
    std::string table2;
    std::string leftColumn;
    std::string rightColumn;
};

struct WhereClause {
    std::string column;
    bool isColumnRhs = false;
    std::string rhs;
};

struct Select {
    std::string tableName;
    std::optional<JoinClause> join;
    std::optional<WhereClause> where;
};
