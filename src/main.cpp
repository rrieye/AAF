#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include "cli.hpp"
#include "command.hpp"
#include "parser.hpp"
#include "db.hpp"
#include "format.hpp"

int main() 
{
    Database db;
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
            
            if (cmd.type == cmd_create)
            {
                std::cout << db.create_table(cmd.create) << "\n";
            }
            else if (cmd.type == cmd_insert)
            {
                std::cout << db.insert(cmd.insert) << "\n";
            }
            else
            {
                Result res = db.select(cmd.select);
                std::string output = format_table(res);
                if (!output.empty()) 
                {
                    std::cout << output << "\n";
                }
            }
        }
        catch (const ParseError& e)
        {
            std::cout << "Parse error: " << e.what() << "\n";
        }
        catch (const DbError& e)
        {
            std::cout << "Database error: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye (: !\n";
    return 0;
}