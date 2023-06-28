#include "rules.hpp"
#include <algorithm>

using namespace draughts;

Alliance Rules::FirstMoveAlliance() const
{
    return Alliance::LIGHT;
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
    if((piece.GetAlliance() == Alliance::LIGHT && ey == 0)
            || (piece.GetAlliance() == Alliance::DARK && ey == BOARD_SIZE - 1))
    {
        coronation = true;
    }
    return coronation;
}

bool Rules::IsSubset(const Move &first, const Move &second) const
{
    const int stepCountFirst = first.StepCount();
    const int stepCountSecond = second.StepCount();

    if(stepCountFirst == stepCountSecond)
        return false;

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
    std::sort(moves.begin(), moves.end(), [](const Move& move1, const Move& move2){
        return move1.StepCount() < move2.StepCount();
    });
    auto it = std::remove_if(moves.begin(), moves.end(), [&](auto &move)
    {
        for(int i = 0; i < moves.size(); ++i)
            if(IsSubset(move, moves[i]))
                return true;
        return false;
    });

    moves.erase(it, moves.end());
}
