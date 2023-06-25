#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardPortuguese : public Board
    {
    public:
        BoardPortuguese();
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual void SetupInitialPosition() override;
        virtual int GetPieceRows() const override;
        virtual Notation GetNotation() const override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        bool MakeMove(const Move &move) override;
        virtual void Reset() override;
    public:
        enum
        {
            BOARD_SIZE = 8,
            NUM_PIECE_ROWS = 3,
        };
    protected:
        int _count12 = 0;
        int _count20 = 0;
        std::map<unsigned int, int> _mapRep;
        std::map<int, std::string> _mapNotation;
    private:
        bool IsPieceOnTheMainDiagonal(const Piece& piece) const;
    };
}
