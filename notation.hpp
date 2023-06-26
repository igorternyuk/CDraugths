#pragma once
#include <map>
#include <string>

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
