#pragma once
#include "board_international.h"

namespace draughts
{
    class BoardBrazilian: public BoardInternational
    {
    public:
        BoardBrazilian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile& tile) const override;
    public:
        enum
        {
            BOARD_SIZE = 8,
            BOARD_WIDTH = 8,
            BOARD_HEIGHT = 8,
            NUM_PIECE_ROWS = 3,
        };
    private:
    };
}
