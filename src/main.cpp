#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "cli.hpp"
#include <cctype>
#include "command.hpp"
#include "parser.hpp"

void print_command(const Command& cmd)
{
    if (cmd.type == cmd_create)
    {
        std::cout << "CREATE: table '" << cmd.create.tableName << "', columns:";
        for (size_t i = 0; i < cmd.create.columns.size(); i++)
        {
            std::cout << " " << cmd.create.columns[i].name;
            if (cmd.create.columns[i].indexed)
            {
                std::cout << " (indexed)";
            }
        }
        std::cout << "\n";
    }
    else if (cmd.type == cmd_insert)
    {
        std::cout << "INSERT: table '" << cmd.insert.tableName << "', values:";
        for (size_t i = 0; i < cmd.insert.values.size(); i++)
        {
            std::cout << " \"" << cmd.insert.values[i] << "\"";
        }
        std::cout << "\n";
    }
    else
    {
        std::cout << "SELECT: table '" << cmd.select.tableName << "'";
        if (cmd.select.join)
        {
            std::cout << ", FULL_JOIN '" << cmd.select.join->table2 << "' ON "
                      << cmd.select.join->leftColumn << " = " << cmd.select.join->rightColumn;
        }
        if (cmd.select.where)
        {
            std::cout << ", WHERE " << cmd.select.where->column << " = ";
            if (cmd.select.where->isColumnRhs)
            {
                std::cout << cmd.select.where->rhs;
            }
            else
            {
                std::cout << "\"" << cmd.select.where->rhs << "\"";
            }
        }
        std::cout << "\n";
    }
}

int main() 
{
    std::cout << "Database CLI started. Type 'EXIT;' to quit.\n";

    while (true) 
    {
        std::cout << "> ";
        auto cmd_opt = read_command();

        if (!cmd_opt.has_value()) 
        {
            break;
        }

        std::string cmd_text = cmd_opt.value();

        if (cmd_text.empty()) 
        {
            continue; 
        }

        std::string check_cmd = cmd_text;
        
        if (!check_cmd.empty() && check_cmd.back() == ' ') 
        {
            check_cmd.pop_back();
        }

        std::transform(check_cmd.begin(), check_cmd.end(), check_cmd.begin(), [](unsigned char c) 
        { 
            return std::tolower(c); 
        });

        if (check_cmd == "exit") 
        {
            break;
        }
      
      try 
        {
            Command cmd = parse(cmd_text);
            print_command(cmd);
        }
        catch (const ParseError& e)
        {
                  std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye (: !\n";
    return 0;
}
