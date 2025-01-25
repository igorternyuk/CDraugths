#pragma once
#include "board_ru.h"

namespace draughts
{
    class BoardDiagonal64: public BoardRu
    {
    public:
        BoardDiagonal64();
    public:
        void SetupInitialPosition() override;
        virtual bool IsCoronationTile(int row, int col, Alliance alliance) const override;
    };
}
