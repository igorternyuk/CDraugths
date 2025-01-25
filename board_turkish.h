#pragma once
#include "board.h"

namespace draughts
{
    class BoardTurkish : public Board
    {
    public:
        BoardTurkish();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
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
            COUNT_2 = 2,
        };
    protected:
        std::map<int,std::pair<int, bool>> _mapDrawRep
                {
                        {COUNT_2, {0, false}},
                };
    protected:
        void SetupTestPosition();
    };
}
