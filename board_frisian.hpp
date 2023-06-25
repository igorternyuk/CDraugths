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
        };
    protected:
        int _count2 = 0;
        int _count7 = 0;
        bool _bCount2Changed = false;
        bool _bCount7Changed = false;
        std::map<int, std::string> _mapNotation;
    };
}

