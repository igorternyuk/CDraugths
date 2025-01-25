#pragma once
#include "board.h"

namespace draughts
{
    class BoardItalian : public Board
    {
    public:
        BoardItalian();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual void SetupInitialPosition() override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        bool MakeMove(const Move &move) override;
        bool UndoLastMove() override;
        virtual void Reset() override;
    public:
        enum
        {
            BOARD_SIZE = 8,
            PIECE_ROWS = 3,
            COUNT_40 = 40,
        };
    protected:
        void SetupTestPosition();
        int _oldBalance = 0;
        std::map<int,std::pair<int, bool>> _mapDrawRep
                {
                        {COUNT_40, {0, false}},
                };
    };
}

