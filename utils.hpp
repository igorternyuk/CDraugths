#pragma once

#include <vector>
#include <string>

class Utils
{
public:
    static std::vector<std::string> split(const std::string& input, const std::string& token);
    static unsigned int hash_str(const char* s);
};

