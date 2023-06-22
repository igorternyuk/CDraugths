#include "rules_draughts64.hpp"
#include <algorithm>

RulesDraughts64::RulesDraughts64()
{

}

Alliance RulesDraughts64::FirstMoveAlliance() const
{
    return Alliance::RED;
}

int RulesDraughts64::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE : PIECE_VALUE;
}

bool RulesDraughts64::IsTileValid(const Position& position, const Tile& tile) const
{
    /*bool rc = false;
    if(tile.IsDark())
    {
        Alliance turn = position.GetTurn();
        const int row = tile.GetRow();
        const int col = tile.GetCol();
        if(position.GetTile(row, col) == tile)
        {
            if(tile.IsEmpty())
                rc = true;
            else
            {
                const Piece& piece = tile.GetPiece();
                rc = piece.GetAlliance() == turn;
            }
        }
    }*/

    return tile.IsDark();
}

void RulesDraughts64::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
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
        int k = 1;
        auto it = std::remove_if(moves.begin(), moves.end(), [&](auto &move)
        {
            for(int i = k; i < moves.size(); ++i)
            {
                if(IsSubset(move, moves[i]))
                {
                    ++k;
                    return true;
                }
            }
            ++k;
            return false;
        });

        moves.erase(it, moves.end());
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
                for(int n = 1; n < BOARD_SIZE; ++n)
                {
                    int nx = x + n * offsetX_[dir];
                    int ny = y + n * offsetY_[dir];
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

bool RulesDraughts64::CheckIfCoronate(const Position &position, const Move &move) const
{
    bool coronation = false;
    const int BOARD_SIZE = position.GetBoardSize();
    const Step& step = move.GetLastStep();
    const int ey = step.GetEnd().GetRow();
    const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
    if((piece.GetAlliance() == Alliance::RED && ey == 0)
            || (piece.GetAlliance() == Alliance::BLUE && ey == BOARD_SIZE - 1))
    {
        coronation = true;
    }
    return coronation;
}

GameStatus RulesDraughts64::GetGameStatus(const Position &position) const
{
    return GameStatus::PLAY;
}

void RulesDraughts64::CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    int N = (piece.IsKing() || move.IsCoronation()) ? position.GetBoardSize() - 1 : 2;

    for(int dir = 0; dir < 4; ++dir)
    {
        bool targetDetected = false;
        Tile current = position.GetTile(py, px);
        Piece target;

        for (int n = 1; n <= N; ++n)
        {
            int dx = n * offsetX_[dir];
            int dy = n * offsetY_[dir];
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
                    Piece p(current.GetRow(), current.GetCol(), piece.GetAlliance(), false);
                    if(CheckIfCoronate(position, move))
                    {
                        p.Crown();
                        move.SetCoronation(true);
                    }
                    legalMoves.push_back(move);
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
