#pragma once
#include "piece.hpp"

class Tile
{
public:
    static Tile NULL_TILE;
    Tile(int row = -1, int y = -1, bool isDark = false);
    bool operator==(const Tile &other) const;
    bool operator!=(const Tile &other) const;
    virtual bool IsLight() const;
    bool IsDark() const;
    void SetDark();
    bool IsEmpty() const;
    bool IsValid() const;
    bool HasPiece() const;
    void SetPiece(const Piece& piece);
    void RemovePiece();
    int GetRow() const;
    void SetRow(int newRow);
    int GetCol() const;
    void SetCol(int newCol);
    const Piece& GetPiece() const;
    Piece &GetPiece();

private:
    int _row;
    int _col;
    bool _isDark;
    Piece _piece;
};

