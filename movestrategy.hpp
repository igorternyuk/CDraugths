#pragma once
#include "player.hpp"
#include "position.hpp"
#include <memory>

namespace draughts
{
    class MoveStrategy
    {
    public:
        virtual ~MoveStrategy() {}
    public:
         virtual Move GetBestMove(std::shared_ptr<Position> position, const Player& player) = 0;
    };
}
