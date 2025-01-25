#pragma once

#include "tile.h"

namespace draughts
{
    class Step
    {
    public:

        Step(const Tile& start, const Tile& end, Piece captured = Piece());

        bool isJump() const;

        bool operator==(const Step &other) const;
        bool operator!=(const Step &other) const;

        const Tile &GetStart() const;
        Tile &GetStart();
        //void SetStart(const Tile &newStart);
        const Tile &GetEnd() const;
        Tile &GetEnd();
        //void SetEnd(const Tile &newEnd);
        const Piece &GetCaptured() const;
        void SetCaptured(const Piece &newCaptured);

    private:
        Tile _start, _end;
        Piece captured;
    };
}
