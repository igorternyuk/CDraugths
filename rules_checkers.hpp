#pragma once
#include "rules.hpp"

namespace draughts
{
    class RulesCheckers: public Rules
    {
    public:
        RulesCheckers();
    public:
        virtual Alliance FirstMoveAlliance() const override;
        virtual int GetPieceValue(const Piece& piece) const override;
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}
