#include "rules_turkish.hpp"
#include <algorithm>
#include <iostream>

using namespace draughts;

RulesTurkish::RulesTurkish()
{

}

Alliance RulesTurkish::FirstMoveAlliance() const
{
    return Alliance::LIGHT;
}

int RulesTurkish::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE : PIECE_VALUE;
}

bool RulesTurkish::IsTileValid(const Position &position, const Tile &tile) const
{
    return tile.IsDark() || tile.IsLight();
}

void RulesTurkish::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
{
    const int BOARD_SIZE = position.GetBoardSize();
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

            const int maxCapture = moves[0].StepCount();
            auto it_ = std::remove_if(moves.begin(), moves.end(), [&](Move &move)
            {
                return move.StepCount() < maxCapture;
            });

            moves.erase(it_, moves.end());
        }

        return;
    }

    const int dy = DirectionOfAlliance(alliance);

    for(const auto& [i,p]: pieces)
    {
        const Piece& piece = *p;
        const int x = piece.GetCol();
        const int y = piece.GetRow();
        const Tile& currTile = position.GetTile(y, x);
        if(piece.IsKing())
        {
            for(int dir = 4; dir < 8; ++dir)
            {
                for(int n = 1; n < BOARD_SIZE; ++n)
                {
                    int nx = x + n * _offsetX[dir];
                    int ny = y + n * _offsetY[dir];
                    const Tile& tile = position.GetTile(ny, nx);
                    if(!tile.IsValid() || !tile.IsEmpty())
                        break;
                    Move move;
                    Step step(currTile, tile);
                    move.AddStep(step);
                    moves.push_back(move);
                }
            }
        }
        else
        {
            for(int dir = 4; dir < 8; ++dir)
            {
                if(-dy == _offsetY[dir])
                    continue;
                int dx = _offsetX[dir];
                int nx = x + dx;
                int ny = y + _offsetY[dir];
                const Tile& tile = position.GetTile(ny, nx);
                bool bIsTileValid = tile.IsValid();
                if(bIsTileValid && tile.IsEmpty())
                {
                    Move move;
                    Step step(currTile, tile);
                    move.AddStep(step);
                    if(CheckIfCoronate(position, move))
                        move.SetCoronation(true);
                    moves.push_back(move);
                }
            }
        }
    }
}

void RulesTurkish::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
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

    //std::cout << position.ToString() << std::endl;
    for(int dir = 4; dir < 8; ++dir)
    {
        if(dir == opposit_dir)
            continue;
        bool targetDetected = false;
        Tile current = position.GetTile(py, px);
        Piece target;
        int N = (piece.IsKing() || move.IsCoronation()) ? position.GetBoardSize() - 1 : 2;
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
                    legalMoves.push_back(move);
                    Piece p(current.GetRow(), current.GetCol(), piece.GetAlliance(), piece.IsKing());
                    CalcAllJumps(position, p, move, legalMoves);
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
