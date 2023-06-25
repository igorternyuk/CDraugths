#pragma once
#include "board_international.hpp"

namespace draughts
{
    class BoardCanadian: public BoardInternational
    {
    public:
        BoardCanadian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual int GetNumPiecesForRow() const override;
        virtual Notation GetNotation() const override;
    public:
        enum
        {
            BOARD_SIZE = 12,
            NUM_PIECE_ROWS = 5,
        };
    };
}


