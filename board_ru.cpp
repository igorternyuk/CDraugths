#include "board_ru.h"
#include "rules_ru.h"
#include <sstream>
#include <iostream>

using namespace draughts;

BoardRu::BoardRu(size_t width, size_t height): Board(std::make_shared<RulesRu>(), width, height)
{
    const size_t HEIGHT = GetBoardHeight();
    const size_t WIDTH = GetBoardWidth();
    for(size_t r = 0; r < HEIGHT; ++r)
        for(size_t c = 0; c < WIDTH; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
            }
        }
    SetupInitialPosition();
}

void BoardRu::SetupInitialPosition()
{
    Board::SetupInitialPosition();
    //SetupTestPosition();
}

void BoardRu::Reset()
{
    Board::Reset();
    for(auto& [k,p]: _mapDrawRep)
    {
        p.first = 0;
        p.second = false;
    }
    _oldBalance = 0;
    _mapRep.clear();
}

void BoardRu::SetupTestPosition()
{
    Reset();
    Clear();

    const std::string board_map[8][8]
            {
                    {"[ ]","[d]","[.]","[d]","[ ]","[.]","[ ]","[.]"},
                    {"[.]","[ ]","[.]","[ ]","[.]","[ ]","[L]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[d]","[ ]","[ ]"},
                    {"[ ]","[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[d]","[ ]","[.]"},
                    {"[.]","[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]","[ ]"},
                    {"[.]","[ ]","[.]","[ ]","[.]","[ ]","[l]","[ ]"}
            };

    for(int y = 0; y < BOARD_SIZE_DEFAULT; ++y)
    {
        for(int x = 0; x < BOARD_SIZE_DEFAULT; ++x)
        {
            if(board_map[y][x] == "[d]")
                SetPiece(y,x,Alliance::DARK, false);
            else if(board_map[y][x] == "[D]")
                SetPiece(y,x,Alliance::DARK, true);
            else if(board_map[y][x] == "[l]")
                SetPiece(y,x,Alliance::LIGHT, false);
            else if(board_map[y][x] == "[L]")
                SetPiece(y,x,Alliance::LIGHT, true);
        }
    }
}

std::shared_ptr<Position> BoardRu::MakeCopy() const
{
    return std::make_shared<BoardRu>(*this);
}

int BoardRu::GetPieceRows() const
{
    return NUM_PIECES_FOR_ROW;
}

Notation BoardRu::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

bool BoardRu::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
        _hash = GetHash();
        _mapRep[_hash]++;
        /////////////////////////////////////////////////////////////////////
        int aCount[4];
        CalcPieceCount(aCount);

        if((aCount[DARK_PIECE] == 0 && aCount[DARK_KING] >= 3 && aCount[LIGHT_KING] == 1)
           || (aCount[LIGHT_PIECE] == 0 && aCount[LIGHT_KING] >= 3 && aCount[LIGHT_PIECE] == 0))
        {
            ++_mapDrawRep[COUNT_15].first;
            _mapDrawRep[COUNT_15].second = true;
        }
        else
            _mapDrawRep[COUNT_15].second = false;

        int reds_total = aCount[DARK_PIECE] + aCount[DARK_KING] ;
        int blues_total = aCount[LIGHT_PIECE] + aCount[LIGHT_KING];
        int total = reds_total + blues_total;

        int balance = (aCount[DARK_PIECE] + 3 * aCount[DARK_KING] ) - (aCount[LIGHT_PIECE] + 3 * aCount[LIGHT_KING]);
        if(total == 2 || total == 3)
        {
            //_mapDrawRep[COUNT_45].first = 0;
            //_mapDrawRep[COUNT_67].first = 0;
            if(balance != _oldBalance)
            {
                _oldBalance = balance;
                _mapDrawRep[COUNT_23].first = 0;
            }
            else
            {
                ++_mapDrawRep[COUNT_23].first;
            }
            _mapDrawRep[COUNT_23].second = true;
        }
        else if(total == 4 || total == 5)
        {
            if(balance != _oldBalance)
            {
                _oldBalance = balance;
                _mapDrawRep[COUNT_45].first = 0;
            }
            else
            {
                ++_mapDrawRep[COUNT_45].first;
            }
            _mapDrawRep[COUNT_45].second = true;
        }
        else if(total == 6 || total == 7)
        {
            if(balance != _oldBalance)
            {
                _oldBalance = balance;
                _mapDrawRep[COUNT_67].first = 0;
            }
            else
            {
                ++_mapDrawRep[COUNT_67].first;
            }
            _mapDrawRep[COUNT_67].second = true;
        }

        if(move.IsJump() || !piece.IsKing())
            _mapDrawRep[COUNT_30].first = 0;
        else
            ++_mapDrawRep[COUNT_30].first;

        _mapDrawRep[COUNT_30].second = true;

        return true;
    }

    return false;
}

bool BoardRu::UndoLastMove()
{
    _hash = GetHash();
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        _mapRep[_hash]--;
    }

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

GameStatus BoardRu::GetGameStatus() const
{
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        if(_mapRep.at(_hash) >= 3)
        {
            //std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(_mapDrawRep.at(COUNT_15).first >= 2 * COUNT_15)
    {
        std::cout << "No king capture within 15 moves!\n";
        return GameStatus::DRAW;
    }

    if(_mapDrawRep.at(COUNT_23).first >= 2 * COUNT_23)
    {
        std::cout << "No captures or coronations in the last 5 moves for 2-3 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(_mapDrawRep.at(COUNT_45).first >= 2 * COUNT_45)
    {
        std::cout << "No captures or coronations in the last 30 moves for 4-5 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(_mapDrawRep.at(COUNT_67).first >= 2 * COUNT_67)
    {
        std::cout << "No captures or coronations in the last 60 moves for 6-7 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(_moveLog.size() >= 2 * COUNT_30)
    {
        if(_mapDrawRep.at(COUNT_30).first >= 2 * COUNT_30)
        {
            std::cout << "Draw because of more then 30 sequential kings moves!\n";
            return GameStatus::DRAW;
        }
    }

    return Board::GetGameStatus();
}

std::string BoardRu::TileToNotation(const Tile &tile) const
{
    int row = tile.GetRow();
    int col = tile.GetCol();
    return _mapAlgebraicNotaion[col] + std::to_string(BOARD_SIZE_DEFAULT - row);
}

