#include "board_international.h"
#include "rules_international.h"
#include <iostream>

using namespace draughts;




BoardInternational::BoardInternational(size_t N): Board(std::make_shared<RulesInternational>(), N)
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
            _mapRepLight[_hash]++;
        else if(piece.GetAlliance() == Alliance::LIGHT)
            _mapRepDark[_hash]++;
        int aCount[4];
        CalcPieceCount(aCount);

        int reds_total = aCount[DARK_PIECE] + aCount[DARK_KING] ;
        int blues_total = aCount[LIGHT_PIECE] + aCount[LIGHT_KING];
        int total = reds_total + blues_total;

        if(piece.IsKing() && !move.IsJump())
            ++_mapDrawRep[COUNT_25].first;
        else
            _mapDrawRep[COUNT_25].first = 0;

        _mapDrawRep[COUNT_25].second = true;

        if((reds_total == 3 && aCount[LIGHT_PIECE] == 0 && aCount[LIGHT_KING] == 1)
           || (blues_total == 3 && aCount[DARK_PIECE] == 0 && aCount[DARK_KING] == 1))
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
        if(!_mapRepLight.empty() && _mapRepLight.find(_hash) != _mapRepLight.end() )
        {
            if(piece.GetAlliance() == Alliance::DARK)
                _mapRepLight[_hash]--;
        }
        if(!_mapRepDark.empty() && _mapRepDark.find(_hash) != _mapRepDark.end() )
        {
            if(piece.GetAlliance() == Alliance::LIGHT)
                _mapRepDark[_hash]--;
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

GameStatus BoardInternational::GetGameStatus() const
{
    if(!_mapRepLight.empty() && _mapRepLight.find(_hash) != _mapRepLight.end() )
    {
        if(_mapRepLight.at(_hash) >= 3)
        {
            // if(_log)
            //     std::cout << "Draw by threefold repetition!\n";
            return GameStatus::DRAW;
        }
    }

    if(!_mapRepDark.empty() && _mapRepDark.find(_hash) != _mapRepDark.end() )
    {
        if(_mapRepDark.at(_hash) >= 3)
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
    _mapRepLight.clear();
    _mapRepDark.clear();
}

void BoardInternational::SetupTestPosition()
{
    Reset();
    Clear();

    const std::string board_map[10][10]
            {
                    {"[ ]","[d]","[.]","[d]","[ ]","[d]","[ ]","[.]","[ ]","[.]"},
                    {"[.]","[ ]","[d]","[ ]","[d]","[ ]","[d]","[ ]","[d]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]","[d]","[ ]","[.]"},
                    {"[.]","[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]","[.]","[ ]","[.]"},
                    {"[.]","[ ]","[d]","[ ]","[d]","[ ]","[.]","[ ]","[ ]","[ ]"},
                    {"[ ]","[.]","[ ]","[.]","[ ]","[l]","[ ]","[ ]","[ ]","[l]"},
                    {"[l]","[ ]","[.]","[ ]","[l]","[ ]","[.]","[ ]","[l]","[ ]"},
                    {"[ ]","[.]","[ ]","[l]","[ ]","[.]","[ ]","[.]","[ ]","[.]"},
                    {"[.]","[ ]","[.]","[ ]","[l]","[ ]","[l]","[ ]","[l]","[ ]"},
            };

    for(int y = 0; y < 10; ++y)
    {
        for(int x = 0; x < 10; ++x)
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

void BoardInternational::SetupBoard()
{
    const size_t HEIGHT = GetBoardHeight();
    const size_t WIDTH = GetBoardWidth();
    int k = 0;
    for(size_t r = 0; r < HEIGHT; ++r)
        for(size_t c = 0; c < WIDTH; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
                ++k;
            }
        }
}
