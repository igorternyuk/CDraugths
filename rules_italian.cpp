#include "rules_italian.h"
#include <algorithm>

using namespace draughts;

RulesItalian::RulesItalian()
{

}

void RulesItalian::PossibleDirections(const Piece &piece, bool isJump, std::vector<int> &dirs) const
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
        }
        else if(piece.GetAlliance() == Alliance::DARK)
        {
            dirs.push_back(eRIGHT_DOWN);
            dirs.push_back(eLEFT_DOWN);
        }
    }
}

int RulesItalian::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE / 2 : PIECE_VALUE;
}

void RulesItalian::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
{
    moves.clear();

    const int BOARD_SIZE = position.GetBoardSize();

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
            std::sort(moves.begin(), moves.end(), [&](const auto& m1, const auto& m2){
                int cnt1 = m1.StepCount();
                int cnt2 = m2.StepCount();
                if(cnt1 == cnt2) //quality rule
                {
                    int numKings1 = 0, numKings2 = 0;
                    for(int i = 0; i < m1.StepCount(); ++i)
                    {
                        const Step& step1 = m1.GetStep(i);
                        numKings1 += !!step1.GetCaptured().IsKing();
                        const Step& step2 = m1.GetStep(i);
                        numKings2 += !!step2.GetCaptured().IsKing();
                    }

                    if(numKings1 == numKings2)
                    {
                        int val1 = 0, val2 = 0;
                        for(int i = 0; i < m1.StepCount(); ++i)
                        {
                            const Step& step1 = m1.GetStep(i);
                            val1 += GetPieceValue(step1.GetStart().GetPiece());
                            const Step& step2 = m1.GetStep(i);
                            val2 += GetPieceValue(step2.GetStart().GetPiece());
                        }

                        return val1 > val2;
                    }
                    return numKings1 > numKings2;
                }
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
        const int N = 1;
        for(const auto& dir: dirs)
        {
            for(int n = 1; n <= N; ++n)
            {
                int dx = n * _offsetX[dir];
                int dy = n * _offsetY[dir];
                int nx = x + dx;
                int ny = y + dy;
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
}

void RulesItalian::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    int N = 2;
    std::vector<int> dirs;
    PossibleDirections(piece, true, dirs);

    for(auto& dir: dirs)
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
                continue;

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
                    if(!p.IsKing() && CheckIfCoronate(position, move))
                    {
                        p.Crown();
                        move.SetCoronation(true);
                        legalMoves.push_back(move);
                        break;
                    }
                    else
                    {
                        legalMoves.push_back(move);
                        CalcAllJumps(position, p, move, legalMoves);
                        legalMoves.push_back(move);
                        move = oldMove;
                    }
                }
            }
            else
            {
                if(current.HasPiece())
                {
                    if(current.GetPiece().GetAlliance() != piece.GetAlliance())
                    {
                        if(current.GetPiece().IsKing())
                            targetDetected = true;
                        else
                            targetDetected = !current.GetPiece().IsKing();
                        if(targetDetected)
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
