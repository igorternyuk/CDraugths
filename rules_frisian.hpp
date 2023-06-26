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
        virtual int GetPieceValue(const Piece& piece) const override;
    protected:
        enum
        {
            MAX_SEQUENTIONAL_KING_MOVES = 3,
        };
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    protected:
        static inline const double MAN_KING_RATIO = 5.5 / 3; // 1.83333333
    };
}
