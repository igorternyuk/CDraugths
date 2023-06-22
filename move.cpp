#include "move.hpp"

Move::Move(): _isCoronation(false), _status(Move::Status::ILLEGAL_MOVE)
{

}

bool Move::operator==(const Move &other) const
{
    const size_t count1 = StepCount();
    const size_t count2 = other.StepCount();
    if(count1 != count2)
        return false;
    for(size_t i = 0; i < count1; ++i)
        if(GetStep(i) != other.GetStep(i))
            return false;
    return true;
}

bool Move::operator!=(const Move &other) const
{
    return !(*this == other);
}

bool Move::IsEmpty() const
{
    return _steps.empty();
}

bool Move::IsJump() const
{
    if(!IsEmpty())
    {
        const Step& stepFirst = GetFirstStep();
        return stepFirst.isJump();
    }
    return false;
}

bool Move::IsRegular() const
{
    return !IsEmpty() && !IsJump();
}

bool Move::IsCoronation() const
{
    return _isCoronation;
}

void Move::SetCoronation(bool isCoronation)
{
    _isCoronation = isCoronation;
}

void Move::UpdateStatus()
{
    const int numSteps = StepCount();
    if(numSteps == 0)
        _status = Status::ILLEGAL_MOVE;
    else if(numSteps == 1)
        _status = GetFirstStep().isJump() ? Status::SINGLE_JUMP : Status::REGULAR_MOVE;
    else
        _status = Status::JUMP_SEQUENCE;
}

size_t Move::StepCount() const
{
    return _steps.size();
}

void Move::AddStep(const Step &step)
{
    _steps.push_back(step);
}

void Move::Clear()
{
    _steps.clear();
}

const Step& Move::GetStep(int i) const
{
    return _steps.at(i);
}

const Step& Move::GetFirstStep() const
{
    return _steps.front();
}

const Step &Move::GetLastStep() const
{
    return _steps.back();
}

const Move::Status &Move::GetStatus() const
{
    return _status;
}
