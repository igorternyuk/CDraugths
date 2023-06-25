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
        virtual std::string TileToNotation(const Tile& tile) const override;
        virtual GameStatus GetGameStatus() const override;
        virtual void SetupInitialPosition() override final;
        virtual void Reset() override;
    private:
        void SetupTestPosition();
    protected:
        int _count25 = 0;
        int _count16 = 0;
        int _count5 = 0;
        std::map<unsigned int, int> _mapRepBlue;
        std::map<unsigned int, int> _mapRepRed;
        std::map<int, std::string> _mapNotation;
    };
}
