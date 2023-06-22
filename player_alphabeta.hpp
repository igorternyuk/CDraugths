#pragma once
#include "player_cpu.hpp"

class PlayerAlphaBeta : public PlayerCPU
{
public:
    PlayerAlphaBeta(Alliance alliance, int depth = 10);
};
