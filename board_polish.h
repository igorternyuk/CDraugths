#pragma once
#include "board_international.h"

namespace draughts
{
    class BoardPolish: public BoardInternational
    {
    public:
        BoardPolish();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile &tile) const override;
    public:
        enum
        {
            BOARD_SIZE = 10,
            NUM_PIECE_ROWS = 4,
        };
    };
}
