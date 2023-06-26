#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardFrisian : public Board
    {
    public:
        BoardFrisian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        virtual void SetupInitialPosition() override final;
        virtual void Reset() override;
        virtual int GetBoardSize() const override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
    private:
        void SetupTestPosition();
    protected:
        enum
        {
            BOARD_SIZE = 10,
            NUM_PIECE_ROW = 4,
            COUNT_2 = 2,
            COUNT_7 = 7,
        };
    protected:
        std::map<int,std::pair<int, bool>> _mapDrawRep {{COUNT_2, {0, false}}, {COUNT_7, {0, false}}};
        std::map<int, std::string> _mapNotation;
    };
}

