#pragma once
#include "step.hpp"
#include <vector>

class Move
{
public:
    Move();
    enum class Status
    {
        REGULAR_MOVE,
        SINGLE_JUMP,
        JUMP_SEQUENCE,
        ILLEGAL_MOVE
    };

public:
    bool operator==(const Move &other) const;
    bool operator!=(const Move &other) const;
    void AddStep(const Step& step);
    void Clear();
    bool IsEmpty() const;
    bool IsJump() const;
    bool IsRegular() const;
    bool IsCoronation() const;
    void SetCoronation(bool isCoronation = true);
    void UpdateStatus();
    size_t StepCount() const;
    const Step& GetStep(int i) const;
    const Step& GetFirstStep() const;
    const Step &GetLastStep() const;
    const Status& GetStatus() const;
private:
    bool _isCoronation;
    Status _status;
    std::vector<Step> _steps;
};
