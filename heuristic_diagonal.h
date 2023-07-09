#pragma once
#include "evaluation_strategy.h"

namespace draughts
{
    class HeuristicDiagonal : public EvaluationStrategy
    {
    public:
        HeuristicDiagonal();
        virtual int Evaluate(const Position& position, const Rules& rules) override;
    };
}
