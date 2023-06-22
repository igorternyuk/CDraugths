#pragma once

#include "tile.hpp"

class Step
{
public:

    Step(Tile start, Tile end, Piece captured = Piece());

    bool isJump() const;

    bool operator==(const Step &other) const;
    bool operator!=(const Step &other) const;

    const Tile &GetStart() const;
    void SetStart(const Tile &newStart);
    const Tile &GetEnd() const;
    void SetEnd(const Tile &newEnd);
    const Piece &GetCaptured() const;
    void SetCaptured(const Piece &newCaptured);

private:
    Tile start, end;
    Piece captured;
};
