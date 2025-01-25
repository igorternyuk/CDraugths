#pragma once
#include "player.h"
#include "movestrategy.h"

namespace draughts
{
    class PlayerCPU : public Player
    {
    public:
        PlayerCPU(Alliance alliance, std::shared_ptr<MoveStrategy> strategy);
    public:
        virtual Move MakeMove(const Position& position) override;
        virtual bool IsHuman() const override;
    private:
        std::shared_ptr<MoveStrategy> _moveStrategy;
    };
}


