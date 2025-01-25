#pragma once
#include "board.h"

namespace draughts
{
    class BoardRu : public Board
    {
    public:
        BoardRu(size_t width = BOARD_SIZE_DEFAULT, size_t height = BOARD_SIZE_DEFAULT);
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
            BOARD_SIZE_DEFAULT = 8,
            NUM_PIECES_FOR_ROW = 3,
            COUNT_23 = 5,
            COUNT_45 = 30,
            COUNT_67 = 60,
            COUNT_15 = 15,
            COUNT_30 = 30,
        };
    protected:
        void SetupTestPosition();
    protected:
        std::map<int,std::pair<int, bool>> _mapDrawRep
                {
                        {COUNT_23, {0, false}},
                        {COUNT_45, {0, false}},
                        {COUNT_67, {0, false}},
                        {COUNT_15, {0, false}},
                        {COUNT_30, {0, false}},
                };
        int _oldBalance = 0;
        std::map<unsigned int, int> _mapRep;
    private:
    };
}

