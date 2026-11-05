#include "query.hpp"

namespace
{

int find_table_column(const Table& table, const std::string& name)
{
    for (size_t i = 0; i < table.columns.size(); i++)
    {
        if (table.columns[i].name == name)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int find_result_column(const Result& res, const std::string& name)
{
    int found = -1;
    int count = 0;

    for (size_t i = 0; i < res.header.size(); i++)
    {
        if (res.header[i] == name)
        {
            found = static_cast<int>(i);
            count++;
        }
    }

    if (count == 0)
    {
        throw DbError("Column " + name + " does not exist");
    }
    if (count > 1)
    {
        throw DbError("Column " + name + " is ambiguous, it exists in both tables");
    }
    return found;
}

}

Result table_to_result(const Table& table)
{
    Result res;

    for (size_t i = 0; i < table.columns.size(); i++)
    {
        res.header.push_back(table.columns[i].name);
    }
    res.rows = table.rows;

    return res;
}

Result full_join(const Table& t1, const Table& t2, const JoinClause& join)
{
    int left = find_table_column(t1, join.leftColumn);
    int right = find_table_column(t2, join.rightColumn);

    if (left < 0 || right < 0)
    {
        // columns written in the opposite order: ON t2_column = t1_column
        int left_swapped = find_table_column(t1, join.rightColumn);
        int right_swapped = find_table_column(t2, join.leftColumn);

        if (left_swapped >= 0 && right_swapped >= 0)
        {
            left = left_swapped;
            right = right_swapped;
        }
    }

    if (left < 0)
    {
        throw DbError("Column " + join.leftColumn + " does not exist in table " + t1.name);
    }
    if (right < 0)
    {
        throw DbError("Column " + join.rightColumn + " does not exist in table " + t2.name);
    }

    Result res;
    for (size_t i = 0; i < t1.columns.size(); i++)
    {
        res.header.push_back(t1.columns[i].name);
    }
    for (size_t i = 0; i < t2.columns.size(); i++)
    {
        res.header.push_back(t2.columns[i].name);
    }

    std::vector<bool> matched(t2.rows.size(), false);
    std::vector<std::string> empty_left(t1.columns.size(), "");
    std::vector<std::string> empty_right(t2.columns.size(), "");

    for (size_t i = 0; i < t1.rows.size(); i++)
    {
        bool has_match = false;

        for (size_t j = 0; j < t2.rows.size(); j++)
        {
            if (t1.rows[i][left] == t2.rows[j][right])
            {
                std::vector<std::string> row = t1.rows[i];
                row.insert(row.end(), t2.rows[j].begin(), t2.rows[j].end());
                res.rows.push_back(row);
                matched[j] = true;
                has_match = true;
            }
        }

        if (!has_match)
        {
            std::vector<std::string> row = t1.rows[i];
            row.insert(row.end(), empty_right.begin(), empty_right.end());
            res.rows.push_back(row);
        }
    }

    for (size_t j = 0; j < t2.rows.size(); j++)
    {
        if (!matched[j])
        {
            std::vector<std::string> row = empty_left;
            row.insert(row.end(), t2.rows[j].begin(), t2.rows[j].end());
            res.rows.push_back(row);
        }
    }

    return res;
}

Result apply_where(const Result& res, const WhereClause& where)
{
    int left = find_result_column(res, where.column);
    int right = -1;

    if (where.isColumnRhs)
    {
        right = find_result_column(res, where.rhs);
    }

    Result filtered;
    filtered.header = res.header;

    for (size_t i = 0; i < res.rows.size(); i++)
    {
        const std::string& value = where.isColumnRhs ? res.rows[i][right] : where.rhs;

        if (res.rows[i][left] == value)
        {
            filtered.rows.push_back(res.rows[i]);
        }
    }

    return filtered;
}
