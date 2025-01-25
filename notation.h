#pragma once
#include <map>
#include <string>

namespace draughts
{
    enum class Notation
    {
        ALGEBRAIC,
        NUMERIC,
    };

    inline static std::map<int, std::string> _mapAlgebraicNotaion
            {
                    { 0, "a"},
                    { 1, "b"},
                    { 2, "c"},
                    { 3, "d"},
                    { 4, "e"},
                    { 5, "f"},
                    { 6, "g"},
                    { 7, "h"},
                    { 8, "i"},
                    { 9, "j"},
            };

    inline static std::map<int, std::string> FillNotationMap(int boardWidth, int boardHeight, bool bBottomLeftColorDark = true, bool bInverse = false)
    {
        int k = bInverse ? boardWidth * boardHeight / 2 + 1 : 0;
        std::map<int, std::string> notationMap;
        for(size_t row = 0; row < boardHeight; ++row)
            for(size_t col = 0; col < boardWidth; ++col)
            {
                bool flag = bBottomLeftColorDark == ((col + row) % 2 != 0);
                if(flag)
                {
                    k += bInverse ? - 1 : +1;
                    int index = row * boardWidth + col;
                    notationMap[index] = std::to_string(k);
                }
            }
        return notationMap;
    }
};
