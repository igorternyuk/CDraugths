#pragma once
#include "board_polish.h"

namespace draughts
{
    class BoardDiagonal100 : public BoardPolish
    {
    public:
        BoardDiagonal100();
    public:
        void SetupInitialPosition() override;
        bool IsCoronationTile(int row, int col, Alliance alliance) const override;
    };
}
