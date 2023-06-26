#include "board_turkish.hpp"
#include "rules_turkish.hpp"
#include <iostream>

using namespace draughts;

BoardTurkish::BoardTurkish(): Board(std::make_shared<RulesTurkish>(), BOARD_SIZE)
{
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
            }
        }
}

void draughts::BoardTurkish::SetupInitialPosition()
{
    const int BOARD_SIZE = GetBoardSize();
    const int NUM_PIECES_FOR_ROW = GetPieceRows();
    Reset();
    Clear();
    for(int y = 1; y <= NUM_PIECES_FOR_ROW; ++y)
        for(int x = 0; x < BOARD_SIZE; ++x)
            SetPiece(y,x,Alliance::DARK);

    for(int y = BOARD_SIZE - 1 - NUM_PIECES_FOR_ROW; y < BOARD_SIZE - 1; ++y)
        for(int x = 0; x < BOARD_SIZE; ++x)
            SetPiece(y,x,Alliance::LIGHT);
}

std::shared_ptr<draughts::Position> draughts::BoardTurkish::MakeCopy() const
{
    return std::make_shared<BoardTurkish>(*this);
}

int draughts::BoardTurkish::GetBoardSize() const
{
    return BOARD_SIZE;
}

int draughts::BoardTurkish::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation draughts::BoardTurkish::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

bool draughts::BoardTurkish::MakeMove(const Move &move)
{
    return Board::MakeMove(move);
}

bool draughts::BoardTurkish::UndoLastMove()
{
    return Board::UndoLastMove();
}

draughts::GameStatus draughts::BoardTurkish::GetGameStatus() const
{
    return Board::GetGameStatus();
}


void draughts::BoardTurkish::Reset()
{
    Board::Reset();
}

void draughts::BoardTurkish::SetupTestPosition()
{

}

std::string draughts::BoardTurkish::TileToNotation(const Tile &tile) const
{
    int row = tile.GetRow();
    int col = tile.GetCol();
    return _mapAlgebraicNotaion[col] + std::to_string(BOARD_SIZE - row);
}
