#pragma once
#include "rules.h"

namespace draughts
{
    class RulesInternational : public Rules
    {
    public:
        RulesInternational();
    public:
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
        virtual void PossibleDirections(const Piece& piece, bool isJump, std::vector<int>& dirs) const override;
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}
