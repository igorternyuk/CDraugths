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
    private:
        void SetupTestPosition();
    protected:
        std::map<int, std::string> _mapNotation;
        std::map<unsigned int, int> _mapRepBlue;
        std::map<unsigned int, int> _mapRepRed;
        int count25 = 0;
        int count16 = 0;
        int count5 = 0;
    };
}
