#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardTurkish : public Board
    {
    public:
        BoardTurkish();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        void SetupInitialPosition() override;
        virtual void Reset() override;
    protected:
        enum
        {
            BOARD_SIZE = 8,
            NUM_PIECE_ROWS = 2,
        };
    protected:
        void SetupTestPosition();
    };
}
