#pragma once
#include "piece.hpp"
#include "position.hpp"
#include "move.hpp"

namespace draughts
{
    class Rules
    {
    public:
        virtual ~Rules() {};
    public:
        virtual Alliance FirstMoveAlliance() const = 0;
        virtual int GetPieceValue(const Piece& piece) const = 0;
        virtual bool IsTileValid(const Position& position, const Tile& tile) const = 0;
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const = 0;
        virtual bool CheckIfCoronate(const Position& position, const Move& move) const = 0;
    protected:
        bool IsSubset(const Move &first, const Move &second) const;
    };
}


