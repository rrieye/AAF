#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "cli.hpp"

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
            std::cout << "Read and formatted: [" << cmd_text << "]\n";
        } 
        catch (const std::exception& e) 
        {
            std::cout << e.what() << "\n";
        }
    }

    std::cout << "Goodbye (: !\n";
    return 0;
}