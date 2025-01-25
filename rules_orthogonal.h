#pragma once
#include "rules.h"

namespace draughts
{
    class RulesOrthogonal : public Rules
    {
    public:
        RulesOrthogonal();
    public:
        virtual Alliance FirstMoveAlliance() const override;
        virtual bool IsTileValid(const Position& position, const Tile& tile) const override;
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
        virtual bool CheckIfCoronate(const Position& position, const Move& move) const override;
    protected:
        void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const override;
    };
}

