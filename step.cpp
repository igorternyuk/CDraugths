#include "step.hpp"

const Tile &Step::GetStart() const
{
    return start;
}

void Step::SetStart(const Tile &newStart)
{
    start = newStart;
}

const Tile &Step::GetEnd() const
{
    return end;
}

void Step::SetEnd(const Tile &newEnd)
{
    end = newEnd;
}

const Piece &Step::GetCaptured() const
{
    return captured;
}

void Step::SetCaptured(const Piece &newCaptured)
{
    captured = newCaptured;
}

Step::Step(Tile start, Tile end, Piece captured):
    start(start), end(end), captured(captured)
{}

bool Step::isJump() const
{
    return captured.GetRow() != -1 && captured.GetCol() != -1; //captured != Piece::NULL_PIECE;//
}

bool Step::operator==(const Step &other) const
{
    if(this->start == other.start && this->end == other.end
            && this->captured == other.captured)
    {
        if(this->start.HasPiece() && other.start.HasPiece())
            return this->start.GetPiece() == other.start.GetPiece();

        if(!this->start.HasPiece() && !other.start.HasPiece())
            return false;
    }

    return false;
}

bool Step::operator!=(const Step &other) const
{
    return !(*this == other);
}
