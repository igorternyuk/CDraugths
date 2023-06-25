#pragma once
#include "board_international.hpp"

namespace draughts
{
    class BoardPolish: public BoardInternational
    {
    public:
        BoardPolish();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
    public:
        enum
        {
            BOARD_SIZE = 10,
            NUM_PIECE_ROWS = 4,
        };
    };
}

