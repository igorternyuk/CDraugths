#pragma once
#include "rules.hpp"

namespace draughts
{
    class RulesFrisian: public Rules
    {
    public:
        RulesFrisian();
    public:
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
    protected:
        enum
        {
            MAX_SEQUENTIONAL_KING_MOVES = 3,
        };
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}
