#pragma once
#include "board_international.hpp"

namespace draughts
{
    class BoardBrazilian: public BoardInternational
    {
    public:
        BoardBrazilian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual int GetNumPiecesForRow() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile& tile) const override;
    public:
        enum
        {
            BOARD_SIZE = 8,
            NUM_PIECE_ROWS = 3,
        };
    private:
        inline static std::map<int, std::string> _mapAlgebraicNotaion
        {
            { 0, "a"},
            { 1, "b"},
            { 2, "c"},
            { 3, "d"},
            { 4, "e"},
            { 5, "f"},
            { 6, "g"},
            { 7, "h"}
        };
    };
}
