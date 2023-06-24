#pragma once
#include "rules.hpp"

namespace draughts
{
    class RulesInternational : public Rules
    {
    public:
        RulesInternational();
    public:
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}
