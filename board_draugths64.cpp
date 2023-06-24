#include "board_draugths64.hpp"
#include "rules_draughts64.hpp"
#include <sstream>
#include <iostream>

using namespace draughts;

static std::map<int, std::string> algebraicNotaionFileMap
{
    { 0, "a"},
    { 1, "b"},
    { 2, "c"},
    { 3, "d"},
    { 4, "e"},
    { 5, "f"},
    { 6, "g"},
    { 7, "h"}
};

BoardDraugths64::BoardDraugths64(): Board(std::make_shared<RulesDraughts64>(), BOARD_SIZE)
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
    SetupInitialPosition();
}

std::shared_ptr<Position> BoardDraugths64::MakeCopy() const
{
    return std::make_shared<BoardDraugths64>(*this);
}

int BoardDraugths64::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardDraugths64::GetNumPiecesForRow() const
{
    return NUM_PIECES_FOR_ROW;
}

Board::Notation BoardDraugths64::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

bool BoardDraugths64::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        _hash = GetHash();
        _mapRep[_hash]++;
        /////////////////////////////////////////////////////////////////////
        int aCount[4];
        CalcPieceCount(aCount);

        if((aCount[RED_PIECE] == 0 && aCount[RED_KING]  >= 3 && aCount[BLUE_KING] == 1)
                || (aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING] >= 3 && aCount[BLUE_PIECE] == 0))
        {
            ++count15;
        }

        int reds_total = aCount[RED_PIECE] + aCount[RED_KING] ;
        int blues_total = aCount[BLUE_PIECE] + aCount[BLUE_KING];
        int total = reds_total + blues_total;

        int balance = (aCount[RED_PIECE] + 3 * aCount[RED_KING] ) - (aCount[BLUE_PIECE] + 3 * aCount[BLUE_KING]);
        if(total == 2 || total == 3)
        {
            count45 = 0;
            count67 = 0;
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count23 = 0;
            }
            else
            {
                ++count23;
            }
        }
        else if(total == 4 || total == 6)
        {
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count45 = 0;
            }
            else
            {
                ++count45;
            }
        }
        else if(total == 6 || total == 7)
        {
            count67 = 0;
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count67 = 0;
            }
            else
            {
                ++count67;
            }
        }

         return true;
    }

    return false;
}

GameStatus BoardDraugths64::GetGameStatus() const
{
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        if(_mapRep.at(_hash) >= 3)
        {
            std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(count15 >= 2 * 15)
    {
        std::cout << "No king capture within 15 moves!\n";
        return GameStatus::DRAW;
    }

    if(count23 >= 2 * 5)
    {
        std::cout << "No captures or coronations in the last 5 moves for 2-3 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(count45 >= 2 * 30)
    {
        std::cout << "No captures or coronations in the last 30 moves for 4-5 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(count67 >= 2 * 60)
    {
        std::cout << "No captures or coronations in the last 60 moves for 6-7 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(_moveLog.size() > 30)
    {
        int count = 0;
        for(auto it = _moveLog.rbegin(); it != _moveLog.rend(); ++it)
        {
            const Move& move = *it;
            for(size_t i = 0; i < move.StepCount(); ++i)
            {
                if(move.GetStep(i).GetStart().GetPiece().IsKing() && !move.IsJump())
                    count++;
            }
        }

        if(count >= 30)
        {
            std::cout << "Draw because of more then 30 sequential kings moves!\n";
            return GameStatus::DRAW;
        }

    }

    return Board::GetGameStatus();
}

std::string BoardDraugths64::TileToNotation(const Tile &tile) const
{
    int row = tile.GetRow();
    int col = tile.GetCol();
    return algebraicNotaionFileMap[col] + std::to_string(BOARD_SIZE - row);
}

