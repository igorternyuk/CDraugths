#include "board_checkers.h"
#include "rules_checkers.h"
#include <iostream>

using namespace draughts;

static std::map<int, std::string> _mapNotation = draughts::FillNotationMap(BoardCheckers::BOARD_SIZE,BoardCheckers::BOARD_SIZE, true, false);

BoardCheckers::BoardCheckers():Board(std::make_shared<RulesCheckers>(), BOARD_SIZE)
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

std::shared_ptr<Position> BoardCheckers::MakeCopy() const
{
    return std::make_shared<BoardCheckers>(*this);
}

void BoardCheckers::SetupInitialPosition()
{
    Board::SetupInitialPosition();
}

int BoardCheckers::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardCheckers::GetNotation() const
{
    return Notation::NUMERIC;
}

std::string BoardCheckers::TileToNotation(const Tile &tile) const
{
    const int row = tile.GetRow();
    const int col = tile.GetCol();
    if(IsValidCoords(row, col))
    {
        const int index = IndexByCoords(row, col);
        if(_mapNotation.find(index) != _mapNotation.end())
            return _mapNotation.at(index);
    }
    return std::string("");
}

GameStatus BoardCheckers::GetGameStatus() const
{
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        if(_mapRep.at(_hash) >= 3)
        {
            //std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    return Board::GetGameStatus();
}

bool BoardCheckers::MakeMove(const Move &move)
{
    //return Board::MakeMove(move);
    if(Board::MakeMove(move))
    {
        _hash = GetHash();
        _mapRep[_hash]++;
        return true;
    }
    return false;
}

bool BoardCheckers::UndoLastMove()
{
    _hash = GetHash();
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        _mapRep[_hash]--;
    }

    return Board::UndoLastMove();
}

void BoardCheckers::Reset()
{
    Board::Reset();
    _mapRep.clear();
}
