#include "board_armenian_tamas.h"
#include "rules_armenian.h"
#include <iostream>
using namespace draughts;

BoardArmenianTamas::BoardArmenianTamas(): Board(std::make_shared<RulesArmenian>(), BOARD_SIZE)
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

void BoardArmenianTamas::SetupInitialPosition()
{
    const int H = GetBoardHeight();
    const int W = GetBoardWidth();
    const int NUM_PIECES_FOR_ROW = GetPieceRows();
    Reset();
    Clear();
    for(int y = 1; y <= NUM_PIECES_FOR_ROW; ++y)
        for(int x = 0; x < W; ++x)
            SetPiece(y,x,Alliance::DARK);

    for(int y = H - 1 - NUM_PIECES_FOR_ROW; y < H - 1; ++y)
        for(int x = 0; x < W; ++x)
            SetPiece(y,x,Alliance::LIGHT);
}

std::shared_ptr<draughts::Position> draughts::BoardArmenianTamas::MakeCopy() const
{
    return std::make_shared<BoardArmenianTamas>(*this);
}

int BoardArmenianTamas::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardArmenianTamas::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

bool BoardArmenianTamas::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        int aCount[4];
        CalcPieceCount(aCount);
        int reds_total = aCount[DARK_PIECE] + aCount[DARK_KING] ;
        int blues_total = aCount[LIGHT_PIECE] + aCount[LIGHT_KING];
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

bool BoardArmenianTamas::UndoLastMove()
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

std::string BoardArmenianTamas::TileToNotation(const Tile &tile) const
{
    int row = tile.GetRow();
    int col = tile.GetCol();
    return _mapAlgebraicNotaion[col] + std::to_string(BOARD_SIZE - row);
}

GameStatus BoardArmenianTamas::GetGameStatus() const
{
    if(_mapDrawRep.at(COUNT_2).first >= 2 * COUNT_2)
    {
        std::cout << "No king capture within 12 moves!\n";
        return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}


void BoardArmenianTamas::Reset()
{
    Board::Reset();
    for(auto& [k,p]: _mapDrawRep)
    {
        p.first = 0;
        p.second = false;
    }
}

void BoardArmenianTamas::SetupTestPosition()
{

}
