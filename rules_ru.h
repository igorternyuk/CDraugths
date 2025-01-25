#pragma once
#include "rules.h"

namespace draughts
{
    class RulesRu: public Rules
    {
    public:
        RulesRu();
    public:
        virtual void PossibleDirections(const Piece& piece, bool isJump, std::vector<int>& dirs) const override;
        virtual void CalcLegalMoves(const Position& move1, Alliance alliance, std::vector<Move> &moves) const override;
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}

