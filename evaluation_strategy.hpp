#pragma once
#include "position.hpp"
#include "rules.hpp"

class EvaluationStrategy
{
public:
    ~EvaluationStrategy() {}
    virtual int Evaluate(const Position& position, const Rules& rules) = 0;
};
