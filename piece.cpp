#include "piece.hpp"

using namespace draughts;

Piece Piece::NULL_PIECE = Piece();

bool Piece::operator==(const Piece &piece) const
{
    return this->_row == piece._row && this->_col == piece._col
            && this->_isKing == piece._isKing && this->_alliance == piece._alliance;
}

bool Piece::operator!=(const Piece &piece) const
{
    return !(*this == piece);
}

Piece::Piece(int row, int col, Alliance alliance, bool isKing):
    _row(row), _col(col), _alliance(alliance), _isKing(isKing)
{
}


int Piece::GetRow() const
{
    return _row;
}

int Piece::GetCol() const
{
    return _col;
}

Alliance Piece::GetAlliance() const
{
    return _alliance;
}

bool Piece::IsKing() const
{
    return _isKing;
}

void Piece::SetRow(int newRow)
{
    _row = newRow;
}

void Piece::SetCol(int newCol)
{
    _col = newCol;
}

void Piece::SetAlliance(Alliance newAlliance)
{
    _alliance = newAlliance;
}

void Piece::Crown()
{
    _isKing = true;
}

void Piece::Uncrown()
{
    _isKing = false;
}

