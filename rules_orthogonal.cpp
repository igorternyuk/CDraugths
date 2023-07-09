#include "rules_orthogonal.h"
#include <algorithm>
#include <iostream>
#include <ostream>

using namespace draughts;

RulesOrthogonal::RulesOrthogonal()
{

}

Alliance RulesOrthogonal::FirstMoveAlliance() const
{
    return Alliance::LIGHT;
}

bool RulesOrthogonal::IsTileValid(const Position &position, const Tile &tile) const
{
    return tile.IsDark() || tile.IsLight();
}

void RulesOrthogonal::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
{
    moves.clear();

    auto pieces = position.GetPieces(alliance);

    for(const auto& [i,p]: pieces)
    {
        Move move;
        CalcAllJumps(position, *p, move, moves);
    }

    if(!moves.empty())
    {
        RemoveSubsets(moves);

        for(auto& m: moves)
        {
            if(CheckIfCoronate(position, m))
                m.SetCoronation(true);
        }

        if(moves.size() > 1)
        {
            //Majority rule
            std::sort(moves.begin(), moves.end(), [&](const Move& m1, const Move& m2){
                int cnt1 = m1.StepCount();
                int cnt2 = m2.StepCount();
                return cnt1 > cnt2;
            });

            const size_t maxCapture = moves[0].StepCount();
            auto it_ = std::remove_if(moves.begin(), moves.end(), [&](Move &move)
            {
                return move.StepCount() < maxCapture;
            });

            moves.erase(it_, moves.end());
        }

        return;
    }

    for(const auto& [i,p]: pieces)
    {
        const Piece& piece = *p;

        std::vector<int> dirs;
        PossibleDirections(piece, false, dirs);

        const int x = piece.GetCol();
        const int y = piece.GetRow();
        const Tile& currTile = position.GetTile(y, x);
        for(auto&& dir: dirs)
            //for(int dir = 4; dir < 8; dir++)
        {
            int nx = x + _offsetX[dir];
            int ny = y + _offsetY[dir];
            const Tile& tile = position.GetTile(ny, nx);
            bool bIsTileValid = tile.IsValid();
            if(bIsTileValid && tile.IsEmpty())
            {
                Move move;
                Step step(currTile, tile);
                move.AddStep(step);
                if(!piece.IsKing() && CheckIfCoronate(position, move))
                    move.SetCoronation(true);
                moves.push_back(move);
            }
        }
    }
}

void RulesOrthogonal::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    int opposit_dir = -1;
    if(!move.IsEmpty())
    {

        const Tile& tile1 = move.GetLastStep().GetStart();
        const Tile& tile2 = move.GetLastStep().GetEnd();
        int dx = tile2.GetCol() - tile1.GetCol();
        int dy = tile2.GetRow() - tile1.GetRow();
        int d = std::max(abs(dx), abs(dy));
        dx /= d;
        dy /= d;
        int dir_index = 0;
        for(int i = 0; i < 8; ++i)
        {
            if(_offsetX[i] == dx && _offsetY[i] == dy)
            {
                dir_index = i;
                break;
            }
        }
        opposit_dir = _oppositeDir[dir_index];
    }

    std::vector<int> dirs;
    PossibleDirections(piece, true, dirs);

    //std::cout << position.ToString() << std::endl;
    for(const auto& dir: dirs)
    {
        if(dir == opposit_dir)
            continue;
        bool targetDetected = false;
        Tile current = position.GetTile(py, px);
        Piece target;
        int N = (piece.IsKing()/* || move.IsCoronation()*/) ? position.GetBoardHeight() - 1 : 2;
        for (int n = 1; n <= N; ++n)
        {
            int dx = n * _offsetX[dir];
            int dy = n * _offsetY[dir];
            int nx = piece.GetCol() + dx;
            int ny = piece.GetRow() + dy;

            current = position.GetTile(ny, nx);
            if(current == Tile::NULL_TILE)
                break;

            if(targetDetected)
            {
                if(current.HasPiece())
                    break;
                bool isSameTarget = false;
                const int stepCount = move.StepCount();
                for(int i = 0; i < stepCount; ++i)
                {
                    const Step& step = move.GetStep(i);
                    if(step.GetCaptured().GetCol() == target.GetCol() && step.GetCaptured().GetRow() == target.GetRow())
                    {
                        isSameTarget = true;
                        break;
                    }
                }
                if(isSameTarget)
                {
                    targetDetected = false;
                    continue;
                }
                else
                {
                    Step step(startTile, current, target);
                    Move oldMove = move;
                    move.AddStep(step);
                    Piece p(current.GetRow(), current.GetCol(), piece.GetAlliance(), piece.IsKing());
                    if(CheckIfCoronate(position, move))
                    {
                        //p.Crown();
                        move.SetCoronation(true);
                    }
                    CalcAllJumps(position, p, move, legalMoves);
                    legalMoves.push_back(move);
                    move = oldMove;
                }
            }
            else
            {
                if(current.HasPiece())
                {
                    if(current.GetPiece().GetAlliance() != piece.GetAlliance())
                    {
                        targetDetected = true;
                        target = current.GetPiece();
                    }
                    else
                    {
                        break;
                    }
                }
                else if(!piece.IsKing())
                {
                    break;
                }
            }
        }
    }
}

bool RulesOrthogonal::CheckIfCoronate(const Position &position, const Move &move) const
{
    bool coronation = false;
    for(size_t i = 0; i < move.StepCount(); ++i)
    {
        const Step& step = move.GetStep(i);
        const int row = step.GetEnd().GetRow();
        const int col = step.GetEnd().GetCol();
        const Piece& piece = move.GetFirstStep().GetStart().GetPiece();

        if(position.IsCoronationTile(row, col, piece.GetAlliance()))
        {
            coronation = true;
            break;
        }
    }

    return coronation;
}
