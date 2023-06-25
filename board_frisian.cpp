#include "board_frisian.hpp"
#include "rules_frisian.hpp"
#include <iostream>

using namespace draughts;

BoardFrisian::BoardFrisian(): Board(std::make_shared<RulesFrisian>(), BOARD_SIZE)
{
    int k = 0;
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
                int index = IndexByCoords(r, c);
                _mapNotation[index] = std::to_string(++k);
            }
        }
}

std::shared_ptr<Position> BoardFrisian::MakeCopy() const
{
    return std::make_shared<BoardFrisian>(*this);
}

bool BoardFrisian::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        int aCount[4];
        CalcPieceCount(aCount);
        int count2Old = _count2;
        int count7Old = _count7;
        if((aCount[RED_KING] == 2 && aCount[RED_PIECE] == 0 && aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING] == 1)
                || (aCount[BLUE_KING] == 2 && aCount[BLUE_PIECE] == 0 && aCount[RED_PIECE] == 0 && aCount[RED_KING] == 1))
        {
            ++_count7;
        }

        if((aCount[RED_KING] == 1 && aCount[RED_PIECE] == 0 && aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING] == 1))
        {
            ++_count2;
        }

        _bCount2Changed = count2Old != _count2;
        _bCount7Changed = count7Old != _count7;

        return true;
    }

    return false;
}

bool BoardFrisian::UndoLastMove()
{
    if(Board::UndoLastMove())
    {
        if(_bCount2Changed)
            --_count2;
        if(_bCount7Changed)
            --_count7;
        return true;
    }

    return false;
}

std::string BoardFrisian::TileToNotation(const Tile &tile) const
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

GameStatus BoardFrisian::GetGameStatus() const
{
    if(_count2 >= 2 * 2)
    {
        std::cout << "Two kings left and they have made by 2 moves!\n";
        return GameStatus::DRAW;
    }

    if(_count7 >= 2 * 7)
    {
        std::cout << "No single king capture within 7 moves with two king!\n";
        return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}

void BoardFrisian::SetupInitialPosition()
{
    Board::SetupInitialPosition();
    //SetupTestPosition();
}

void BoardFrisian::Reset()
{
    Board::Reset();
    _count2 = 0;
    _count7 = 0;
}

int BoardFrisian::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardFrisian::GetPieceRows() const
{
    return NUM_PIECE_ROW;
}

Board::Notation BoardFrisian::GetNotation() const
{
    return Notation::NUMERIC;
}

void BoardFrisian::SetupTestPosition()
{
    Reset();
    Clear();
    SetPiece(4,9,Alliance::RED, true);
    //SetPiece(6,7,Alliance::RED, true);
    //SetPiece(9,2,Alliance::RED, false);
    //SetPiece(9,4,Alliance::RED, false);

    SetPiece(5,4,Alliance::BLUE, true);
    SetPiece(9,2,Alliance::BLUE, false);
    /*SetPiece(8,5,Alliance::BLUE, false);
    SetPiece(1,4,Alliance::BLUE, false);
    SetPiece(3,4,Alliance::BLUE, false);
    SetPiece(7,4,Alliance::BLUE, false);
    SetPiece(9,6,Alliance::BLUE, false);*/
    SetPiece(9,8,Alliance::BLUE, true);
}
