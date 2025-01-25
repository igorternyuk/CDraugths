#pragma once
#include "step.h"
#include <vector>

namespace draughts
{
    class Move
    {
    public:
        Move();
        enum class Status
        {
            REGULAR_MOVE,
            SINGLE_JUMP,
            JUMP_SEQUENCE,
            ILLEGAL_MOVE,
            TO_REMOVE
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
        Step& GetFirstStep();
        const Step &GetLastStep() const;
        const Status& GetStatus() const;
        void MarkToRemove();
    private:
        bool _isCoronation;
        Status _status;
        std::vector<Step> _steps;
    };
}

