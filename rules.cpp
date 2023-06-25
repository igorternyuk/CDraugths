#include "rules.hpp"
#include <algorithm>

using namespace draughts;

Alliance Rules::FirstMoveAlliance() const
{
    return Alliance::BLUE;
}

int Rules::GetPieceValue(const Piece &piece) const
{
    return piece.IsKing() ? KING_VALUE : PIECE_VALUE;
}

bool Rules::IsTileValid(const Position &position, const Tile &tile) const
{
     return tile.IsDark();
}

bool Rules::CheckIfCoronate(const Position &position, const Move &move) const
{
    bool coronation = false;
    const int BOARD_SIZE = position.GetBoardSize();
    const Step& step = move.GetLastStep();
    const int ey = step.GetEnd().GetRow();
    const Piece& piece = move.GetFirstStep().GetStart().GetPiece();
    if((piece.GetAlliance() == Alliance::BLUE && ey == 0)
            || (piece.GetAlliance() == Alliance::RED && ey == BOARD_SIZE - 1))
    {
        coronation = true;
    }
    return coronation;
}

bool Rules::IsSubset(const Move &first, const Move &second) const
{
    const int stepCountFirst = first.StepCount();
    const int stepCountSecond = second.StepCount();

    if(stepCountFirst < stepCountSecond)
    {
        const size_t min = stepCountFirst;
        for(size_t i = 0; i < min; ++i)
        {
            const Step& step1 = first.GetStep(i);
            const Step& step2 = second.GetStep(i);
            if(step1 != step2)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Rules::RemoveSubsets(std::vector<Move> &moves) const
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
}
