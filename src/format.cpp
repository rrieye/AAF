#include "format.hpp"
#include <vector>
#include <sstream>

static size_t text_width(const std::string& s) 
{
    size_t width = 0;
    for (unsigned char c : s) 
    {
        if ((c & 0xC0) != 0x80) 
        {
            width++;
        }
    }
    return width;
}

std::string format_table(const Result& res) 
{
    if (res.header.empty()) return "";

    std::vector<size_t> widths(res.header.size(), 0);
    
    for (size_t i = 0; i < res.header.size(); ++i) 
    {
        widths[i] = text_width(res.header[i]);
    }

    for (const auto& row : res.rows) 
    {
        for (size_t i = 0; i < row.size() && i < widths.size(); ++i) 
        {
            if (text_width(row[i]) > widths[i]) 
            {
                widths[i] = text_width(row[i]);
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
        size_t padding = widths[i] - text_width(res.header[i]);
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
            size_t padding = widths[i] - text_width(cell);
            size_t pad_left = padding / 2;
            size_t pad_right = padding - pad_left;
            
            output << " " << std::string(pad_left, ' ') << cell << std::string(pad_right, ' ') << " |";
        }
        output << "\n";
    }
    
    // the bottom border is printed for an empty result too
    output << separator;

    return output.str();
}
