#pragma once
#include "evaluation_strategy.h"

namespace draughts
{
    class HeuristicDefault : public EvaluationStrategy
    {
    public:
        HeuristicDefault();
        virtual int Evaluate(const Position& position, const Rules& rules) override;
    };
}
