#include "rules_turkish.h"
#include <algorithm>
#include <iostream>

using namespace draughts;

void RulesTurkish::PossibleDirections(const Piece &piece, bool isJump, std::vector<int> &dirs) const
{
    dirs.clear();

    dirs.push_back(eLEFT);
    dirs.push_back(eRIGHT);

    if(piece.IsKing())
    {
        dirs.push_back(eUP);
        dirs.push_back(eDOWN);
    }
    else
    {
        if(piece.GetAlliance() == Alliance::LIGHT)
            dirs.push_back(eUP);
        else if(piece.GetAlliance() == Alliance::DARK)
            dirs.push_back(eDOWN);
    }
}
