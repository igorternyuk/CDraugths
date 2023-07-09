#include "board_italian.h"
#include "rules_italian.h"
#include <algorithm>
#include <iostream>

using namespace draughts;

static std::map<int, std::string> _mapNotation = FillNotationMap(BoardItalian::BOARD_SIZE,BoardItalian::BOARD_SIZE, false, true);

BoardItalian::BoardItalian():Board(std::make_shared<RulesItalian>(), BOARD_SIZE)
{
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 == 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
            }
        }
}

void BoardItalian::SetupInitialPosition()
{
    const int W = GetBoardWidth();
    const int H = GetBoardHeight();
    const int NUM_PIECES_FOR_ROW = GetPieceRows();
    Reset();
    Clear();
    for(int y = 0; y < NUM_PIECES_FOR_ROW; ++y)
        for(int x = y % 2; x < W; x += 2)
            SetPiece(y,x,Alliance::DARK);


    for(int y = H - NUM_PIECES_FOR_ROW; y < H; ++y)
        for(int x = y % 2; x < W; x += 2)
            SetPiece(y,x,Alliance::LIGHT);
}

std::shared_ptr<draughts::Position> draughts::BoardItalian::MakeCopy() const
{
    return std::make_shared<BoardItalian>(*this);
}

int BoardItalian::GetPieceRows() const
{
    return PIECE_ROWS;
}

Notation BoardItalian::GetNotation() const
{
    return Notation::NUMERIC;
}

std::string BoardItalian::TileToNotation(const Tile &tile) const
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

GameStatus BoardItalian::GetGameStatus() const
{
    if(_mapDrawRep.at(COUNT_40).first >= 2 * COUNT_40)
    {
        std::cout << "No captures or coronations in the last 40 moves!\n";
        return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}

bool BoardItalian::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        /////////////////////////////////////////////////////////////////////
        int aCount[4];
        CalcPieceCount(aCount);

        int balance = (aCount[DARK_PIECE] + 3 * aCount[DARK_KING] ) - (aCount[LIGHT_PIECE] + 3 * aCount[LIGHT_KING]);
        if(balance != _oldBalance)
        {
            _oldBalance = balance;
            _mapDrawRep[COUNT_40].first = 0;
        }
        else
        {
            ++_mapDrawRep[COUNT_40].first;
        }

        return true;
    }

    return false;
}

bool BoardItalian::UndoLastMove()
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

void BoardItalian::Reset()
{
    Board::Reset();
    for(auto& [k,p]: _mapDrawRep)
    {
        p.first = 0;
        p.second = false;
    }
    _oldBalance = 0;
}

void BoardItalian::SetupTestPosition()
{

}
