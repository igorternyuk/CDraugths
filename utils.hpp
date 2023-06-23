#pragma once

#include <vector>
#include <string>
#include <functional>
#include "move.hpp"

namespace draughts
{
    class Utils
    {
    public:
        static std::vector<std::string> split(const std::string& input, const std::string& token);
        static unsigned int hash_str(const char* s);
        static bool AreStepsEqual(const Step &first, const Step &second);
        static bool AreMovesEqual(const Move &first, const Move &second, std::function<bool(const Step &,const Step &)> pred = AreStepsEqual);
        static bool IsSubset(const Move &first, const Move &second, std::function<bool(const Step &,const Step &)> pred = AreStepsEqual);
    };
}

