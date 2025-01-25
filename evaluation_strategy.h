#pragma once
#include "position.h"
#include "rules.h"

namespace draughts
{
    class EvaluationStrategy
    {
    public:
        ~EvaluationStrategy() {}
        virtual int Evaluate(const Position& position, const Rules& rules) = 0;
    };
}

