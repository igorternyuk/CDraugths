#include "board_portuguese.hpp"
#include "rules_portuguese.hpp"
#include <algorithm>
#include <iostream>

using namespace draughts;

BoardPortuguese::BoardPortuguese():Board(std::make_shared<RulesPortuguese>(), BOARD_SIZE)
{
    int k = BOARD_SIZE * BOARD_SIZE / 2;
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 == 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
                int index = IndexByCoords(r, c);
                _mapNotation[index] = std::to_string(k--);
            }
        }
}


std::shared_ptr<Position> BoardPortuguese::MakeCopy() const
{
    return std::make_shared<BoardPortuguese>(*this);
}

void BoardPortuguese::SetupInitialPosition()
{
    const int BOARD_SIZE = GetBoardSize();
    const int NUM_PIECES_FOR_ROW = GetPieceRows();
    Reset();
    Clear();
    for(int y = 0; y < NUM_PIECES_FOR_ROW; ++y)
        for(int x = y % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::DARK);


    for(int y = BOARD_SIZE - NUM_PIECES_FOR_ROW; y < BOARD_SIZE; ++y)
        for(int x = y % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::LIGHT);
}

int BoardPortuguese::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardPortuguese::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardPortuguese::GetNotation() const
{
    return Notation::NUMERIC;
}

std::string BoardPortuguese::TileToNotation(const Tile &tile) const
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

GameStatus BoardPortuguese::GetGameStatus() const
{
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        if(_mapRep.at(_hash) >= 3)
        {
            std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(_mapDrawRep.at(COUNT_12).first >= 2 * COUNT_12)
    {
        std::cout << "No king capture within 12 moves!\n";
        return GameStatus::DRAW;
    }

    if(_moveLog.size() >= 2 * COUNT_20)
    {
        if(_mapDrawRep.at(COUNT_20).first >= 2 * COUNT_20)
        {
            std::cout << "Draw because of more then 20 sequential kings moves!\n";
            return GameStatus::DRAW;
        }
    }

    return Board::GetGameStatus();
}

bool BoardPortuguese::MakeMove(const Move &move)
{
    //return Board::MakeMove(move);
    if(Board::MakeMove(move))
    {
        _hash = GetHash();
        _mapRep[_hash]++;

        int aCount[4];
        CalcPieceCount(aCount);

        if((aCount[RED_PIECE] == 0 && aCount[RED_KING]  == 3 && aCount[BLUE_KING] == 1)
                || (aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING]  == 3 && aCount[RED_KING] == 1))
        {
            if(_mapDrawRep.at(COUNT_12).first == 0)
            {
                const auto& redPieces = GetPieces(Alliance::DARK);
                bool bStartCount12 = std::any_of(redPieces.begin(), redPieces.end(), [&](const auto& p)
                {
                    return IsPieceOnTheMainDiagonal(*p.second);
                });
                if(bStartCount12)
                {
                    ++_mapDrawRep[COUNT_12].first;
                    _mapDrawRep[COUNT_12].second = true;
                }
                else
                    _mapDrawRep[COUNT_12].second = false;
            }

            if(_mapDrawRep[COUNT_12].first > 0)
            {
                ++_mapDrawRep[COUNT_12].first;
                _mapDrawRep[COUNT_12].second = true;
            }
            else
                _mapDrawRep[COUNT_12].second = false;
        }

        const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
        if(move.IsJump() || !piece.IsKing())
            _mapDrawRep[COUNT_20].first = 0;
        else
            ++_mapDrawRep[COUNT_20].first;
        _mapDrawRep[COUNT_20].second = true;

        return true;
    }
    return false;
}

bool BoardPortuguese::UndoLastMove()
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

void BoardPortuguese::Reset()
{
    Board::Reset();
    _mapRep.clear();
}

bool BoardPortuguese::IsPieceOnTheMainDiagonal(const Piece &piece) const
{
    return piece.GetRow() == piece.GetCol();
}
