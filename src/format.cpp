#include "format.hpp"
#include <vector>
#include <sstream>

std::string format_table(const Result& res) 
{
    if (res.header.empty()) return "";

    std::vector<size_t> widths(res.header.size(), 0);
    
    for (size_t i = 0; i < res.header.size(); ++i) 
    {
        widths[i] = res.header[i].length();
    }

    for (const auto& row : res.rows) 
    {
        for (size_t i = 0; i < row.size() && i < widths.size(); ++i) 
        {
            if (row[i].length() > widths[i]) 
            {
                widths[i] = row[i].length();
            }
        }
    }

    auto make_separator = [&widths]() 
    {
        std::ostringstream ss;
        ss << "+";
        for (size_t w : widths) 
        {
            ss << std::string(w + 2, '-') << "+";
        }
        ss << "\n";
        return ss.str();
    };

    std::ostringstream output;
    std::string separator = make_separator();

    output << separator << "|";
    for (size_t i = 0; i < res.header.size(); ++i) 
    {
        size_t padding = widths[i] - res.header[i].length();
        size_t pad_left = padding / 2;
        size_t pad_right = padding - pad_left;
        output << " " << std::string(pad_left, ' ') << res.header[i] << std::string(pad_right, ' ') << " |";
    }
    output << "\n" << separator;

    for (const auto& row : res.rows) 
    {
        output << "|";
        for (size_t i = 0; i < res.header.size(); ++i) 
        {
            std::string cell = (i < row.size()) ? row[i] : "";
            size_t padding = widths[i] - cell.length();
            size_t pad_left = padding / 2;
            size_t pad_right = padding - pad_left;
            
            output << " " << std::string(pad_left, ' ') << cell << std::string(pad_right, ' ') << " |";
        }
        output << "\n";
    }
    
    if (!res.rows.empty()) 
    {
        output << separator;
    }

    return output.str();
}