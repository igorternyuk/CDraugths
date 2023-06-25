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
        virtual int GetNumPiecesForRow() const override;
        virtual Board::Notation GetNotation() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        void SetupInitialPosition() override;

    private:
        enum
        {
            BOARD_SIZE = 8,
            NUM_PIECES_FOR_ROW = 3,
        };
    private:
        void SetupTestPosition();
    private:
        int count23 = 0; // 2-3 piece ending limit 5 moves
        int count45 = 0; // 4-5 piece ending limit 30 moves
        int count67 = 0; // 6-7 piece ending limit 60 moves
        int count15 = 0;
        int oldBalance = 0;
        std::map<unsigned int, int> _mapRep;
    private:
        inline static std::map<int, std::string> _mapAlgebraicNotaion
        {
            { 0, "a"},
            { 1, "b"},
            { 2, "c"},
            { 3, "d"},
            { 4, "e"},
            { 5, "f"},
            { 6, "g"},
            { 7, "h"}
        };
    };
}

