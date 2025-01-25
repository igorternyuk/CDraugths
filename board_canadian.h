#pragma once
#include "board_international.h"

namespace draughts
{
    class BoardCanadian: public BoardInternational
    {
    public:
        BoardCanadian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile &tile) const override;
    public:
        enum
        {
            BOARD_SIZE = 12,
            NUM_PIECE_ROWS = 5,
        };
    };
}


