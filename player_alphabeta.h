#pragma once
#include "player_cpu.h"
#include "heuristic_default.h"

namespace draughts
{
    class PlayerAlphaBeta : public PlayerCPU
    {
    public:
        PlayerAlphaBeta(Alliance alliance, int depth = 8, std::shared_ptr<EvaluationStrategy> evaluationStrategy = std::make_shared<HeuristicDefault>());
    };
}
