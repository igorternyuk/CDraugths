#include "step.h"

using namespace draughts;

const Tile &Step::GetStart() const
{
    return _start;
}

/*void Step::SetStart(const Tile &newStart)
{
    _start = newStart;
}*/

const Tile &Step::GetEnd() const
{
    return _end;
}

/*void Step::SetEnd(const Tile &newEnd)
{
    _end = newEnd;
}*/

const Piece &Step::GetCaptured() const
{
    return captured;
}

void Step::SetCaptured(const Piece &newCaptured)
{
    captured = newCaptured;
}

Step::Step(const Tile& start, const Tile& end, Piece captured):
        _start(start), _end(end), captured(captured)
{}

bool Step::isJump() const
{
    return captured.GetRow() != -1 && captured.GetCol() != -1; //captured != Piece::NULL_PIECE;//
}

bool Step::operator==(const Step &other) const
{
    if(this->_start == other._start && this->_end == other._end
       && this->captured == other.captured)
    {
        if(this->_start.HasPiece() && other._start.HasPiece())
            return this->_start.GetPiece() == other._start.GetPiece();

        if(!this->_start.HasPiece() && !other._start.HasPiece())
            return true;
    }

    return false;
}

bool Step::operator!=(const Step &other) const
{
    return !(*this == other);
}

Tile &Step::GetStart() {
    return _start;
}

Tile &Step::GetEnd() {
    return _end;
}
