#pragma once
#include "db.hpp"

Result table_to_result(const Table& table);
Result full_join(const Table& t1, const Table& t2, const JoinClause& join);
Result apply_where(const Result& res, const WhereClause& where);
