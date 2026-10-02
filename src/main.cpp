#include <iostream>
#include <string>
#include <algorithm>
#include "cli.hpp"

int main() 
{
    std::cout << "Database CLI started. Type 'EXIT;' to quit.\n";

    while (true) 
    {
        std::cout << "> ";
        std::string cmd_text = read_command();

        if (cmd_text.empty()) 
        {
            if (std::cin.eof()) 
            {
                break;
            }
            continue; 
        }

        std::string check_cmd = cmd_text;
        if (!check_cmd.empty() && check_cmd.back() == ' ') 
        {
            check_cmd.pop_back();
        }

        std::transform(check_cmd.begin(), check_cmd.end(), check_cmd.begin(), 
            [](unsigned char c){ return std::tolower(c); });

        if (check_cmd == "exit") 
        {
            break;
        }

        std::cout << "Read and formatted: [" << cmd_text << "]\n";
    }

    std::cout << "Goodbye (: !\n";
    return 0;
}