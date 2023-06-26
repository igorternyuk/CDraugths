#include "rules_frisian.hpp"
#include <algorithm>
#include <iostream>

using namespace draughts;

RulesFrisian::RulesFrisian():Rules()
{

}

void RulesFrisian::CalcLegalMoves(const Position &position, Alliance alliance, std::vector<Move> &moves) const
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

        auto MoveValue = [&](const Move& move)
        {
            int quality = 0;
            for(size_t i = 0; i < move.StepCount(); ++i)
            {
                const Step& step = move.GetStep(i);
                quality += GetPieceValue(step.GetCaptured());
            }
            return quality;
        };
        //Majority rule
        std::sort(moves.begin(), moves.end(), [&](const Move& m1, const Move& m2){
            //int cnt1 = m1.StepCount();
            //int cnt2 = m2.StepCount();
            //if(cnt1 == cnt2) //quality rule
            //{
                int quality1 = MoveValue(m1);
                int quality2 = MoveValue(m2);
                if(quality1 == quality2)
                {
                    const Piece& piece1 = m1.GetFirstStep().GetStart().GetPiece();
                    const Piece& piece2 = m2.GetFirstStep().GetStart().GetPiece();
                    return GetPieceValue(piece1) > GetPieceValue(piece2);
                }
                return quality1 > quality2;
            //}
            //return cnt1 > cnt2;
        });

        //const int maxCapture = moves[0].StepCount();
        const int maxValue = MoveValue(moves[0]);
        const Piece& piece1 = moves[0].GetFirstStep().GetStart().GetPiece();
        const int piece1_val = GetPieceValue(piece1);
        auto it_ = std::remove_if(moves.begin(), moves.end(), [&](auto &move)
        {
            //int num_captures = move.StepCount();
            int move_val = MoveValue(move);
            const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
            const int piece_val = GetPieceValue(piece);
            return move_val == maxValue  ? piece_val < piece1_val : move_val < maxValue;
            //return num_captures == maxCapture ? (move_val == maxValue  ? piece_val < piece1_val : move_val < maxValue) : num_captures < maxCapture;
        });

        moves.erase(it_, moves.end());

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

    /*A king may only move three times in a row unless it makes a capture. Otherwise the player must move another piece.
     *   If the player has only kings on the board this rule does not apply.*/
    bool bHasMen = std::any_of(moves.begin(), moves.end(), [](const Move& move){
        return !move.GetFirstStep().GetStart().GetPiece().IsKing();
    });

    if(bHasMen)
    {
        const std::vector<Move> moveLog = position.GetMoveLog();
        if(moveLog.size() > 6)
        {
            int counter3 = 0;
            std::vector<Move> vect;
            bool flag = false;
            for(auto it = moveLog.rbegin(); counter3 != 3 && it != moveLog.rend(); ++it)
            {
                const Piece& piece = it->GetFirstStep().GetStart().GetPiece();
                if(piece.GetAlliance() == alliance && !it->IsJump())
                {
                    if(piece.IsKing())
                    {
                        ++counter3;
                        vect.push_back(*it);
                    }
                    else
                    {
                        flag = true;
                        break;
                    }
                }
            }

            if(!flag && counter3 >= 3)
            {
                bool bHasSameKingMoved3Times = true;
                for(int i = 2; i > 0; --i)
                {
                    const Tile& tile_prev_end = vect[i].GetLastStep().GetEnd();
                    const Tile& tile_curr_start = vect[i-1].GetFirstStep().GetStart();
                    bool eq = (tile_prev_end.GetRow() == tile_curr_start.GetRow()
                               && tile_prev_end.GetCol() == tile_curr_start.GetCol());
                    bHasSameKingMoved3Times &= eq;
                }
                if(bHasSameKingMoved3Times)
                {
                    const Tile& tile_last = vect.front().GetLastStep().GetEnd();
                    const int king_row = tile_last.GetRow();
                    const int king_col = tile_last.GetCol();
                    auto it_ = std::remove_if(moves.begin(), moves.end(), [&](auto &move)
                    {
                        const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
                        return piece.GetRow() == king_row && piece.GetCol() == king_col;
                    });

                    moves.erase(it_, moves.end());
                }
            }
        }
    }
}

int RulesFrisian::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? int(MAN_KING_RATIO * PIECE_VALUE) : PIECE_VALUE;
}

void RulesFrisian::CalcAllJumps(const Position &position, const Piece &piece, Move move, std::vector<Move> &legalMoves) const
{
    int px = piece.GetCol();
    int py = piece.GetRow();
    Tile startTile = position.GetTile(py, px);

    //std::cout << position.ToString() << std::endl;
    for(int dir = 0; dir < 8; ++dir)
    {
        bool targetDetected = false;
        Tile current = position.GetTile(py, px);
        Piece target;
        //dir < 4 diagonal directions
        //dir > 4 vertical and horizontal captures
        bool isDiagonaDir = dir < 4;
        int N = (piece.IsKing() || move.IsCoronation()) ? position.GetBoardSize() - 1 : (isDiagonaDir ? 2 : 4);
        for (int n = 1; n <= N; ++n)
        {
            int dx = n * offsetX_[dir];
            int dy = n * offsetY_[dir];
            int nx = piece.GetCol() + dx;
            int ny = piece.GetRow() + dy;

            current = position.GetTile(ny, nx);
            if(current == Tile::NULL_TILE)
                break;
            if(current.IsLight())
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
                else if(!piece.IsKing() && isDiagonaDir)
                {
                    break;
                }
            }
        }
    }
}
