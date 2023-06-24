#pragma once
#include "player_cpu.hpp"

namespace draughts
{
    class PlayerAlphaBeta : public PlayerCPU
    {
    public:
        PlayerAlphaBeta(Alliance alliance, int depth = 8);
    };
}
