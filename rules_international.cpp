#include "rules_international.h"
#include <algorithm>

using namespace draughts;

RulesInternational::RulesInternational()
{

}

void RulesInternational::PossibleDirections(const Piece &piece, bool isJump, std::vector<int> &dirs) const
{
    if(piece.IsKing())
    {
        dirs.push_back(eRIGHT_UP);
        dirs.push_back(eLEFT_UP);
        dirs.push_back(eRIGHT_DOWN);
        dirs.push_back(eLEFT_DOWN);
    }
    else
    {
        if(piece.GetAlliance() == Alliance::LIGHT)
        {
            dirs.push_back(eRIGHT_UP);
            dirs.push_back(eLEFT_UP);
            if(isJump)
            {
                dirs.push_back(eRIGHT_DOWN);
                dirs.push_back(eLEFT_DOWN);
            }
        }
        else if(piece.GetAlliance() == Alliance::DARK)
        {
            dirs.push_back(eRIGHT_DOWN);
            dirs.push_back(eLEFT_DOWN);
            if(isJump)
            {
                dirs.push_back(eRIGHT_UP);
                dirs.push_back(eLEFT_UP);
            }
        }
    }
}

void RulesInternational::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
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
        if(moves.size() > 1)
        {
            RemoveSubsets(moves);
        }

        for(auto& m: moves)
        {
            if(CheckIfCoronate(position, m))
            {
                m.SetCoronation(true);
                m.GetFirstStep().GetStart().GetPiece().Crown();
            }
        }

        if(moves.size() > 1)
        {
            //Majority rule
            std::sort(moves.begin(), moves.end(), [](const auto& m1, const auto& m2){
                int cnt1 = m1.StepCount();
                int cnt2 = m2.StepCount();
                return cnt1 > cnt2;
            });

            const int maxCapture = moves[0].StepCount();

            auto it_ = std::remove_if(moves.begin(), moves.end(), [maxCapture](auto &move)
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
        const int N = piece.IsKing() ? BOARD_SIZE - 1 : 1;
        for(const auto& dir: dirs)
        {
            for(int n = 1; n <= N; ++n)
            {
                int nx = x + n * _offsetX[dir];
                int ny = y + n * _offsetY[dir];
                const Tile& tile = position.GetTile(ny, nx);
                if(!tile.IsValid() || !tile.IsEmpty())
                    break;
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

void RulesInternational::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    int N = (piece.IsKing() /*|| move.IsCoronation()*/) ? position.GetBoardSize() - 1 : 2;

    std::vector<int> dirs;
    PossibleDirections(piece, true, dirs);

    for(const auto& dir: dirs)
    {
        bool targetDetected = false;
        Tile current = position.GetTile(py, px);
        Piece target;

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
                if(!isSameTarget)
                {
                    Step step(startTile, current, target);
                    Move oldMove = move;
                    move.AddStep(step);
                    Piece p(current.GetRow(), current.GetCol(), piece.GetAlliance(), piece.IsKing());
                    /*if(CheckIfCoronate(position, move))
                    {
                        //p.Crown();
                        move.SetCoronation(true);
                    }*/
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
