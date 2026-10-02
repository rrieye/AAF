#include <iostream>
#include <string>
#include "cli.hpp"

int main() {
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

        if (cmd_text == "EXIT" || cmd_text == "exit") 
        {
            break;
        }

        std::cout << "Read and formatted: [" << cmd_text << "]\n";
    }

    std::cout << "Goodbye (: !\n";
    return 0;
}