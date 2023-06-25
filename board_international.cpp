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
        if(piece.GetAlliance() == Alliance::RED)
            _mapRepBlue[_hash]++;
        else if(piece.GetAlliance() == Alliance::BLUE)
            _mapRepRed[_hash]++;
        int aCount[4];
        CalcPieceCount(aCount);

        int reds_total = aCount[RED_PIECE] + aCount[RED_KING] ;
        int blues_total = aCount[BLUE_PIECE] + aCount[BLUE_KING];
        int total = reds_total + blues_total;

        if(piece.IsKing() && !move.IsJump())
            ++count25;
        else
            count25 = 0;

        if((reds_total == 3 && aCount[BLUE_PIECE] == 0 && aCount[BLUE_KING] == 1)
                || (blues_total == 3 && aCount[RED_PIECE] == 0 && aCount[RED_KING] == 1))
        {
            ++count16;
        }
        else
            count16 = 0;

        if(total == 2)
            ++count5;

        return true;
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
            if(_log)
                std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(!_mapRepRed.empty() && _mapRepRed.find(_hash) != _mapRepRed.end() )
    {
        if(_mapRepRed.at(_hash) >= 3)
        {
            if(_log)
                std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(count25 >= 2 * 25)
    {
        std::cout << "Last 25 moves were made only with kings without captures!\n";
        return GameStatus::DRAW;
    }

    if(count16 >= 2 * 16)
    {
        std::cout << "No win within 16 moves with 3 pieces against single king!\n";
        return GameStatus::DRAW;
    }

    if(count5 >= 2 * 30)
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

void BoardInternational::SetupTestPosition()
{
    Reset();
    Clear();
    SetPiece(2,7,Alliance::RED, true);
    SetPiece(6,7,Alliance::RED, false);
    SetPiece(9,2,Alliance::RED, false);
    SetPiece(9,4,Alliance::RED, false);

    SetPiece(5,4,Alliance::BLUE, false);
    SetPiece(6,5,Alliance::BLUE, false);
    SetPiece(8,5,Alliance::BLUE, false);
}
