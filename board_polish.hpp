#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardPolish : public Board
    {
    public:
        BoardPolish();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual int GetNumPiecesForRow() const override;
        virtual Notation GetNotation() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        virtual void SetupInitialPosition() override;
    private:
        enum
        {
            BOARD_SIZE = 10,
            NUM_PIECES_FOR_ROW = 4,
        };
    private:
        void SetupTestPosition();
    private:
        std::map<int, std::string> _mapNotation;
    };
}
