#include "board_international.hpp"
#include "rules_international.hpp"
#include <iostream>

using namespace draughts;

BoardInternational::BoardInternational(int N): Board(std::make_shared<RulesInternational>(), N)
{

}

bool BoardInternational::MakeMove(const Move &move)
{
    //return Board::MakeMove(move);
    if(Board::MakeMove(move))
    {
        _hash = GetHash();
        const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
        if(piece.GetAlliance() == Alliance::DARK)
            _mapRepBlue[_hash]++;
        else if(piece.GetAlliance() == Alliance::LIGHT)
            _mapRepRed[_hash]++;
        int aCount[4];
        CalcPieceCount(aCount);

        int reds_total = aCount[RED_PIECE] + aCount[RED_KING] ;
        int blues_total = aCount[BLUE_PIECE] + aCount[BLUE_KING];
        int total = reds_total + blues_total;

        if(piece.IsKing() && !move.IsJump())
            ++_mapDrawRep[COUNT_25].first;
        else
            _mapDrawRep[COUNT_25].first = 0;

        _mapDrawRep[COUNT_25].second = true;

        if((reds_total == 3 && aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING] == 1)
                || (blues_total == 3 && aCount[RED_PIECE] == 0 && aCount[RED_KING] == 1))
        {
            ++_mapDrawRep[COUNT_16].first;
            _mapDrawRep[COUNT_16].second = true;
        }
        else
        {
            _mapDrawRep[COUNT_16].second = false;
        }

        if(total == 2)
        {
            ++_mapDrawRep[COUNT_5].first;
            _mapDrawRep[COUNT_5].second = true;
        }
        else
            _mapDrawRep[COUNT_5].second = false;


        return true;
    }

    return false;
}

bool BoardInternational::UndoLastMove()
{
    _hash = GetHash();
    const std::vector<Move>& moveLog = GetMoveLog();
    if(!moveLog.empty())
    {
        const Move& moveLast = GetMoveLog().back();
        const Piece& piece = moveLast.GetFirstStep().GetStart().GetPiece();
        if(!_mapRepBlue.empty() && _mapRepBlue.find(_hash) != _mapRepBlue.end() )
        {
            if(piece.GetAlliance() == Alliance::DARK)
                _mapRepBlue[_hash]--;
        }
        if(!_mapRepRed.empty() && _mapRepRed.find(_hash) != _mapRepRed.end() )
        {
            if(piece.GetAlliance() == Alliance::LIGHT)
                _mapRepRed[_hash]--;
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
    }

    return false;
}

std::string BoardInternational::TileToNotation(const Tile &tile) const
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

GameStatus BoardInternational::GetGameStatus() const
{
    if(!_mapRepBlue.empty() && _mapRepBlue.find(_hash) != _mapRepBlue.end() )
    {
        if(_mapRepBlue.at(_hash) >= 3)
        {
           // if(_log)
           //     std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(!_mapRepRed.empty() && _mapRepRed.find(_hash) != _mapRepRed.end() )
    {
        if(_mapRepRed.at(_hash) >= 3)
        {
            //if(_log)
            //    std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(_mapDrawRep.at(COUNT_25).first >= 2 * COUNT_25)
    {
        std::cout << "Last 25 moves were made only with kings without captures!\n";
        return GameStatus::DRAW;
    }

    if(_mapDrawRep.at(COUNT_16).first >= 2 * COUNT_16)
    {
        std::cout << "No win within 16 moves with 3 pieces against single king!\n";
        return GameStatus::DRAW;
    }

    if(_mapDrawRep.at(COUNT_5).first >= 2 * 30)
    {
        std::cout << "No win within 5 moves with one piece against one!\n";
        return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}

void BoardInternational::SetupInitialPosition()
{
    Board::SetupInitialPosition();
    //SetupTestPosition();
}

void BoardInternational::Reset()
{
    Board::Reset();
    for(auto& [k,p]: _mapDrawRep)
    {
        p.first = 0;
        p.second = false;
    }
    _mapRepBlue.clear();
    _mapRepRed.clear();
}

void BoardInternational::SetupTestPosition()
{
    Reset();
    Clear();
    SetPiece(2,7,Alliance::DARK, true);
    SetPiece(6,7,Alliance::DARK, false);
    SetPiece(9,2,Alliance::DARK, false);
    SetPiece(9,4,Alliance::DARK, false);

    SetPiece(5,4,Alliance::LIGHT, false);
    SetPiece(6,5,Alliance::LIGHT, false);
    SetPiece(8,5,Alliance::LIGHT, false);
}
