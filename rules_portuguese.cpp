#include "rules_portuguese.hpp"
#include <algorithm>

using namespace draughts;

RulesPortuguese::RulesPortuguese()
{

}

Alliance RulesPortuguese::FirstMoveAlliance() const
{
    return Alliance::LIGHT;
}

int RulesPortuguese::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE * 2 / 3 : PIECE_VALUE;
}

void RulesPortuguese::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
{
    moves.clear();

    const int BOARD_SIZE =position.GetBoardSize();

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
                    int quality1 = 0, quality2 = 0;
                    for(int i = 0; i < m1.StepCount(); ++i)
                    {
                        const Step& step1 = m1.GetStep(i);
                        quality1 += GetPieceValue(step1.GetCaptured());
                        const Step& step2 = m1.GetStep(i);
                        quality2 += GetPieceValue(step2.GetCaptured());
                    }

                    return quality1 > quality2;
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

    const int dy = DirectionOfAlliance(alliance);

    for(const auto& [i,p]: pieces)
    {
        const Piece& piece = *p;
        const int x = piece.GetCol();
        const int y = piece.GetRow();
        const Tile& currTile = position.GetTile(y, x);
        int N = piece.IsKing() ? BOARD_SIZE - 1 : 2;
        if(piece.IsKing())
        {
            for(int dir = 0; dir < 4; ++dir)
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

void RulesPortuguese::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    const int BOARD_SIZE = position.GetBoardSize();
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);
    Alliance alliance = piece.GetAlliance();
    int N = piece.IsKing() ? BOARD_SIZE - 1 : 2;
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
                        if(p.IsKing() || move.StepCount() < PIECE_CAPTURE_LIMIT)
                        {
                            CalcAllJumps(position, p, move, legalMoves);
                            legalMoves.push_back(move);
                            move = oldMove;
                        }
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
