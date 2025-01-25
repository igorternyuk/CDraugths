#pragma once
#include "piece.h"
#include "position.h"
#include "move.h"

namespace draughts
{
    class Rules
    {
    public:
        virtual ~Rules() {};
    public:
        virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const = 0;
        virtual Alliance FirstMoveAlliance() const;
        virtual int GetPieceValue(const Piece& piece) const;
        virtual bool IsTileValid(const Position& position, const Tile& tile) const;
        virtual bool CheckIfCoronate(const Position& position, const Move& move) const;
        virtual void PossibleDirections(const Piece& piece, bool isJump, std::vector<int>& dirs) const;
    protected:
        virtual void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const = 0;
        bool IsSubset(const Move &first, const Move &second) const;
        void RemoveSubsets(std::vector<Move>& moves) const;

    protected:
        enum PieceValues
        {
            PIECE_VALUE = 100,
            KING_VALUE = 300,
        };

        enum DirIndices
        {
            eRIGHT_UP,
            eLEFT_UP,
            eRIGHT_DOWN,
            eLEFT_DOWN,
            eLEFT,
            eRIGHT,
            eDOWN,
            eUP,
        };

        inline static constexpr int _offsetX[8] { +1, -1, +1, -1, -1, +1, 0, 0 };
        inline static constexpr int _offsetY[8] { -1, -1, +1, +1, 0, 0, +1, -1 };
        inline static constexpr int _oppositeDir[8] { 3, 2, 1, 0, 5, 4, 7, 6 };
    };
}


