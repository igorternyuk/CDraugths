#pragma once
#include "board.h"

namespace draughts
{
    class BoardCheckers : public Board
    {
    public:
        BoardCheckers();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual void SetupInitialPosition() override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        bool MakeMove(const Move &move) override;
        virtual bool UndoLastMove() override;
        virtual void Reset() override;
    public:
        enum
        {
            BOARD_SIZE = 8,
            NUM_PIECE_ROWS = 3,
        };
    protected:
        std::map<unsigned int, int> _mapRep;
    };
}
