#pragma once
#include "player.h"

namespace draughts
{
    class PlayerHuman : public Player
    {
    public:
        PlayerHuman(Alliance alliance);
        virtual Move MakeMove(const Position& position) override;
        virtual bool IsHuman() const override;
        virtual void Select(const Position& position, int x, int y);
        virtual const std::vector<std::pair<int, int>>& GetSelection() const;
        virtual void Unselect();
    private:
        std::vector<std::pair<int, int>> _selectedSequence;
    };
}


