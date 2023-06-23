#pragma once
#include "evaluation_strategy.hpp"

namespace draughts
{
    class EuristicDefault : public EvaluationStrategy
    {
    public:
        EuristicDefault();
        virtual int Evaluate(const Position& position, const Rules& rules) override;
    };
}
