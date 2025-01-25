#pragma once
#include "board.h"

namespace draughts
{
    class BoardInternational : public Board
    {
    public:
        BoardInternational(size_t N);
    public:
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual GameStatus GetGameStatus() const override;
        virtual void SetupInitialPosition() override;
        virtual void Reset() override;
    protected:
        virtual void SetupBoard();
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
        std::map<unsigned int, int> _mapRepLight;
        std::map<unsigned int, int> _mapRepDark;
    };
}
