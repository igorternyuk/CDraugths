#include "tile.hpp"
#include <cmath>

using namespace draughts;

Tile Tile::NULL_TILE = Tile();

Tile::Tile(int row, int col, bool isDark):
    _row(row), _col(col), _isDark(isDark)
{}


bool Tile::operator==(const Tile &other) const
{
    return this->_row == other._row && this->_col == other._col
            && this->_piece == other._piece;
}

bool Tile::operator!=(const Tile &other) const
{
    return !(*this == other);
}

bool Tile::IsEmpty() const
{
    return _piece.GetAlliance() == Alliance::NONE;
}

bool Tile::IsValid() const
{
    return *this != NULL_TILE;
}

bool Tile::HasPiece() const
{
    return !IsEmpty();
}

bool Tile::IsLight() const
{
    return !_isDark;
    //return (_col + _row) % 2 == 0;
}

bool Tile::IsDark() const
{
    return _isDark;
}

void Tile::SetDark()
{
    _isDark = true;
}

void Tile::SetPiece(const Piece& piece)
{
    this->_piece = piece;
    this->_piece.SetRow(_row);
    this->_piece.SetCol(_col);
}

void Tile::RemovePiece()
{
    this->_piece.GetAlliance();
    this->_piece = Piece::NULL_PIECE;
}

int Tile::GetRow() const
{
    return _row;
}

void Tile::SetRow(int newRow)
{
    _row = newRow;
}


int Tile::GetCol() const
{
    return _col;
}

void Tile::SetCol(int newCol)
{
    _col = newCol;
}

const Piece& Tile::GetPiece() const
{
    return _piece;
}

Piece& Tile::GetPiece()
{
    return _piece;
}
