#include "rules_checkers.hpp"
#include <algorithm>

using namespace draughts;

RulesCheckers::RulesCheckers()
{

}

Alliance RulesCheckers::FirstMoveAlliance() const
{
    return Alliance::DARK;
}

int RulesCheckers::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE / 2 : PIECE_VALUE;
}

void RulesCheckers::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
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
        if(moves.size() > 1)
            RemoveSubsets(moves);

        for(auto& m: moves)
        {
            if(CheckIfCoronate(position, m))
                m.SetCoronation(true);
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
            for(int dir = 0; dir < 4; ++dir)
            {
                for(int n = 1; n <= 1; ++n)
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
            int nx = 0, ny = 0;

            for(int dx = -1; dx <= +1; dx += 2)
            {
                nx = x + dx;
                ny = y + dy;
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

void RulesCheckers::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    Alliance alliance = piece.GetAlliance();
    int N = 2;
    std::vector<int> indices;
    if(alliance == Alliance::DARK || piece.IsKing())
    {
        indices.push_back(eRIGHT_DOWN);
        indices.push_back(eLEFT_DOWN);
    }
    if(alliance == Alliance::LIGHT || piece.IsKing())
    {
        indices.push_back(eRIGHT_UP);
        indices.push_back(eLEFT_UP);
    }

    for(auto& dir: indices)
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
                    if(CheckIfCoronate(position, move))
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
