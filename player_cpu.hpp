#pragma once
#include "player.hpp"
#include "movestrategy.hpp"

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

