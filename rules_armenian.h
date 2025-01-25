#pragma once
#include "rules_orthogonal.h"

namespace draughts
{
    class RulesArmenian : public RulesOrthogonal
    {
    public:
        RulesArmenian();
    public:
        virtual void PossibleDirections(const Piece& piece, bool isJump, std::vector<int>& dirs) const;
    };
}
