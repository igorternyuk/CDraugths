#pragma once
#include "board.hpp"

namespace draughts
{
    class BoardInternational : public Board
    {
    public:
        BoardInternational(int N);
    public:        
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        virtual void SetupInitialPosition() override final;
        virtual void Reset() override;
    private:
        void SetupTestPosition();
    protected:
        enum
        {
            COUNT_5 = 5,
            COUNT_16 = 16,
            COUNT_25 = 25,
        };
    protected:
        std::map<int,std::pair<int, bool>> _mapDrawRep
        {
            {COUNT_5, {0, false}},
            {COUNT_16, {0, false}},
            {COUNT_25, {0, false}}
        };
        std::map<unsigned int, int> _mapRepBlue;
        std::map<unsigned int, int> _mapRepRed;
        std::map<int, std::string> _mapNotation;
    };
}
