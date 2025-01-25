#pragma once
#include "alliance.h"

namespace draughts
{
    class Piece
    {
    public:

        Piece(int row = -1, int col = -1, Alliance alliance = Alliance::NONE, bool isKing = false);
        static Piece NULL_PIECE;
        bool operator==(const Piece &piece) const;
        bool operator!=(const Piece &piece) const;
        int GetRow() const;
        int GetCol() const;
        Alliance GetAlliance() const;
        bool IsKing() const;
        void Crown();
        void Uncrown();
        void SetRow(int newX);
        void SetCol(int newY);
        void SetAlliance(Alliance newAlliance);

    protected:
        int _row, _col;
        Alliance _alliance;
        bool _isKing;
    };
}

