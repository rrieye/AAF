#pragma once
#include <string>
#include <optional>
#include <istream>

std::optional<std::string> read_command(std::istream& in);