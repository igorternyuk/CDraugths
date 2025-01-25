#pragma once
#include "alliance.h"
#include "move.h"
#include "position.h"

namespace draughts
{
    class Player
    {
    public:
        Player(Alliance alliance);
    public:
        virtual Move MakeMove(const Position& position) = 0;
        virtual bool IsHuman() const = 0;
        virtual Alliance GetAlliance() const;
        virtual Alliance GetOpponentAlliance() const;
    private:
        Alliance _alliance;
    };
}
