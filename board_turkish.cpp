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
    if(Board::MakeMove(move))
    {
        int aCount[4];
        CalcPieceCount(aCount);
        int reds_total = aCount[RED_PIECE] + aCount[RED_KING] ;
        int blues_total = aCount[BLUE_PIECE] + aCount[BLUE_KING];
        int total = reds_total + blues_total;
        if(total == 2)
        {
            ++_mapDrawRep[COUNT_2].first;
            _mapDrawRep[COUNT_2].second = true;
        }
        else
            _mapDrawRep[COUNT_2].second = false;

        return true;
    }
    return false;
}

bool draughts::BoardTurkish::UndoLastMove()
{
    if(Board::UndoLastMove())
    {
        for(auto& [k, p]: _mapDrawRep)
        {
            if(p.first > 0)
                --p.first;
            p.second = false;
        }

        return true;
    }
    return false;
}

draughts::GameStatus draughts::BoardTurkish::GetGameStatus() const
{
    if(_mapDrawRep.at(COUNT_2).first >= 2 * COUNT_2)
    {
        std::cout << "No king capture within 12 moves!\n";
        return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}


void draughts::BoardTurkish::Reset()
{
    Board::Reset();
    for(auto& [k,p]: _mapDrawRep)
    {
        p.first = 0;
        p.second = false;
    }
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
