#include "rules_ru.h"
#include <algorithm>

using namespace draughts;


RulesRu::RulesRu()
{

}

void RulesRu::PossibleDirections(const Piece &piece, bool isJump, std::vector<int> &dirs) const
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

void RulesRu::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
{
    const int BOARD_SIZE = position.GetBoardHeight();
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

            for(auto& move1: moves)
            {
                if(move1.GetStatus() == Move::Status::TO_REMOVE)
                    continue;
                bool flag = false;
                const Piece& piece1 = move1.GetFirstStep().GetStart().GetPiece();
                if(piece1.IsKing())
                {
                    int dy1 = move1.GetFirstStep().GetEnd().GetRow() - move1.GetFirstStep().GetStart().GetRow();
                    int dx1 = move1.GetFirstStep().GetEnd().GetCol() - move1.GetFirstStep().GetStart().GetCol();
                    for(Move& move2: moves)
                    {
                        if(move2.GetStatus() == Move::Status::TO_REMOVE)
                            continue;
                        if(move1 == move2)
                            continue;
                        if(move2.GetFirstStep().GetStart().HasPiece())
                        {
                            const Piece& piece2 = move2.GetFirstStep().GetStart().GetPiece();
                            if(piece2 == piece1)
                            {
                                int dy2 = move2.GetFirstStep().GetEnd().GetRow() - move2.GetFirstStep().GetStart().GetRow();
                                int dx2 = move2.GetFirstStep().GetEnd().GetCol() - move2.GetFirstStep().GetStart().GetCol();
                                if(dx1 * dx2 > 0 && dy1 * dy2 > 0)
                                {
                                    if(move1.StepCount() == 1 && move2.StepCount() > 1)
                                    {
                                        move1.MarkToRemove();
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            auto newEnd = std::remove_if(moves.begin(), moves.end(), [&](const Move& move1){
                return move1.GetStatus() == Move::Status::TO_REMOVE;
            });
            moves.erase(newEnd, moves.end());
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

void RulesRu::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    int N = (piece.IsKing() || move.IsCoronation()) ? position.GetBoardSize() - 1 : 2;

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
                    if(CheckIfCoronate(position, move))
                    {
                        p.Crown();
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
