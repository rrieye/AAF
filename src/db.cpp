#include "db.hpp"
#include "query.hpp"

std::string Database::create_table(const Create& cmd) 
{
    if (tables_.find(cmd.tableName) != tables_.end()) 
    {
        throw DbError("Table " + cmd.tableName + " already exists");
    }

    Table new_table;
    new_table.name = cmd.tableName;
    new_table.columns = cmd.columns;

    tables_.emplace(cmd.tableName, std::move(new_table));

    return "Table " + cmd.tableName + " has been created";
}

std::string Database::insert(const Insert& cmd) 
{
    auto it = tables_.find(cmd.tableName);
    if (it == tables_.end()) 
    {
        throw DbError("Table " + cmd.tableName + " does not exist");
    }

    Table& table = it->second;

    if (cmd.values.size() != table.columns.size()) 
    {
        throw DbError("Insert error: expected " + std::to_string(table.columns.size()) + 
                      " values, but got " + std::to_string(cmd.values.size()));
    }

    table.rows.push_back(cmd.values);

    return "1 row has been inserted into " + cmd.tableName;
}

Result Database::select(const Select& cmd) 
{
    auto it = tables_.find(cmd.tableName);
    if (it == tables_.end()) 
    {
        throw DbError("Table " + cmd.tableName + " does not exist");
    }

    Result res;

    if (cmd.join) 
    {
        auto it2 = tables_.find(cmd.join->table2);
        if (it2 == tables_.end()) 
        {
            throw DbError("Table " + cmd.join->table2 + " does not exist");
        }
        res = full_join(it->second, it2->second, *cmd.join);
    } 
    else 
    {
        res = table_to_result(it->second);
    }

    if (cmd.where) 
    {
        res = apply_where(res, *cmd.where);
    }

    return res;
}
