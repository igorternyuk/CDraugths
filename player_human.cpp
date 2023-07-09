#include "player_human.h"
#include "utils.h"

using namespace draughts;

PlayerHuman::PlayerHuman(Alliance alliance): Player(alliance)
{
}

Move PlayerHuman::MakeMove(const Position &position)
{
    Move emptyMove;
    const size_t sz = _selectedSequence.size();
    if(sz > 1)
    {
        Move move;
        for(size_t si = 1; si < sz; ++si)
        {
            const int row_ = _selectedSequence[si - 1].first;
            const int col_ = _selectedSequence[si - 1].second;
            const int row = _selectedSequence[si].first;
            const int col = _selectedSequence[si].second;
            const Tile& tileStart = position.GetTile(row_, col_);
            const Tile& tileEnd = position.GetTile(row, col);
            Step step(tileStart, tileEnd);
            move.AddStep(step);
        }
        std::vector<Move> lolm;
        position.LegalMoves(position.GetTurn(), lolm);
        const int movesAvailable = lolm.size();
        bool isSubset = false;
        for(int li = 0; li < movesAvailable; ++li)
        {
            Move& lm = lolm[li];
            if(Utils::AreMovesEqual(move,lm))
            {
                lm.UpdateStatus();
                Unselect();
                return lm;
            }
            isSubset |= Utils::IsSubset(move, lm);
        }

        if(!isSubset)
            Unselect();
        /*for(int li = 0; li < movesAvailable; ++li)
        {
            Move& lm = lolm[li];
            if(lm.StepCount() == sz - 1)
            {
                bool bMoveFound = true;
                for(size_t si = 1; si < sz; ++si)
                {
                    const int row_ = _selectedSequence[si - 1].first;
                    const int col_ = _selectedSequence[si - 1].second;
                    const int row = _selectedSequence[si].first;
                    const int col = _selectedSequence[si].second;
                    const Step& step = lm.GetStep(si-1);
                    if(step.GetStart().GetRow() != row_ || step.GetStart().GetCol() != col_
                            || step.GetEnd().GetRow() != row || step.GetEnd().GetCol() != col)
                    {
                        bMoveFound = false;
                    }
                }

                if(bMoveFound)
                {
                    lm.UpdateStatus();
                    Unselect();
                    return lm;
                }
            }
        }*/
    }

    return emptyMove;
}

bool PlayerHuman::IsHuman() const
{
    return true;
}

void PlayerHuman::Select(const Position& position, int x, int y)
{
    const int row = y;
    const int col = x;
    bool rc = false;
    if(position.HasAvailableTile(row, col))
    {
        const Tile& tile = position.GetTile(row, col);
        if(_selectedSequence.empty())
        {
            if(tile.HasPiece() && tile.GetPiece().GetAlliance() == position.GetTurn())
                rc = true;
        }
        else
        {
            if(!tile.HasPiece())
                rc = true;
        }
        _selectedSequence.emplace_back(row, col);
    }
    if(!rc)
        Unselect();
}

const std::vector<std::pair<int, int> > &PlayerHuman::GetSelection() const
{
    return _selectedSequence;
}

void PlayerHuman::Unselect()
{
    _selectedSequence.clear();
}
