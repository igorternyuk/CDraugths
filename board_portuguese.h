#pragma once
#include "board.h"

namespace draughts
{
    class BoardPortuguese : public Board
    {
    public:
        BoardPortuguese();
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
            NUM_PIECE_ROWS = 3,
            COUNT_12 = 12,
            COUNT_20 = 20,
        };
    protected:
        std::map<int,std::pair<int, bool>> _mapDrawRep
                {
                        {COUNT_12, {0, false}},
                        {COUNT_20, {0, false}},
                };
        std::map<unsigned int, int> _mapRep;
    private:
        bool IsPieceOnTheMainDiagonal(const Piece& piece) const;
    };
}
