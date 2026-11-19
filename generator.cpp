#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

void generate_script(int count, const std::string& filename, bool indexed) 
{
    std::ofstream out(filename);
    if (!out) 
    {
        std::cout << "Error creating file: " << filename << "\n";
        return;
    }

    std::string idx = indexed ? " INDEXED" : "";

    out << "CREATE owners (owner_id" << idx << ", owner_name, owner_age);\n";
    out << "CREATE cats (cat_id" << idx << ", cat_owner_id" << idx << ", cat_name);\n";

    for (int i = 0; i < count; i++) 
    {
        out << "INSERT owners (\"" << i << "\", \"Owner_" << i << "\", \"30\");\n";
        out << "INSERT cats (\"" << i << "\", \"" << i << "\", \"Cat_" << i << "\");\n";
    }

    out << "SELECT FROM owners FULL_JOIN cats ON owner_id = cat_owner_id WHERE cat_name = \"Cat_" << (count / 2) << "\";\n";
    
    out << "EXIT;\n";
    out.close();
    
    std::cout << "Generated: " << filename << " (" << count << " rows)\n";
}

int main() 
{
    std::filesystem::create_directories("benchmarks");
    std::cout << "Generating benchmark scripts in 'benchmarks' folder...\n";
    
    generate_script(1000, "benchmarks/bench_1k_no_idx.txt", false);
    generate_script(10000, "benchmarks/bench_10k_no_idx.txt", false);
    generate_script(100000, "benchmarks/bench_100k_no_idx.txt", false);
    
    generate_script(1000, "benchmarks/bench_1k_idx.txt", true);
    generate_script(10000, "benchmarks/bench_10k_idx.txt", true);
    generate_script(100000, "benchmarks/bench_100k_idx.txt", true);

    std::cout << "Done!\n";
    return 0;
}