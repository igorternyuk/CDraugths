#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardDraugths64 : public Board
    {
    public:
        BoardDraugths64();
    public:
        virtual std::shared_ptr<Position> MakeCopy() const override;
        virtual int GetBoardSize() const override;
        virtual bool IsEndgameScenario() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual std::string TileToAlgebraicNotation(const Tile& tile) const override;
        virtual std::string MoveToAlgebraicNotation(const Move& move) const override;
        virtual std::string ToString() const override;
        virtual int GetTotalPieces() const override;
        void SetupInitialPosition() override final;
        virtual GameStatus GetGameStatus() const override;
    private:
        enum
        {
            BOARD_SIZE = 8,
            NUM_CHECKER_ROW_FOR_ONE_SIDE = 3,
        };
    private:
        int count23 = 0; // 2-3 piece ending limit 5 moves
        int count45 = 0; // 4-5 piece ending limit 30 moves
        int count67 = 0; // 6-7 piece ending limit 60 moves
        int count15 = 0;
        int oldBalance = 0;
        std::map<unsigned int, int> _mapRep;
    };
}

