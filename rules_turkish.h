#pragma once
#include "rules_orthogonal.h"

namespace draughts
{
    class RulesTurkish: public RulesOrthogonal
    {
    public:
        virtual void PossibleDirections(const Piece& piece, bool isJump, std::vector<int>& dirs) const override;
    };
}
