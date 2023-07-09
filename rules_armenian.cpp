#include "rules_armenian.h"

using namespace draughts;

RulesArmenian::RulesArmenian():RulesOrthogonal()
{

}

void draughts::RulesArmenian::PossibleDirections(const Piece &piece, bool isJump, std::vector<int> &dirs) const
{
    if(isJump)
    {
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
    else
    {
        if(piece.IsKing())
        {
            for(size_t i = 0; i < 8; ++i)
                dirs.push_back(i);
        }
        else
        {
            dirs.push_back(eLEFT);
            dirs.push_back(eRIGHT);
            if(piece.GetAlliance() == Alliance::LIGHT)
            {
                dirs.push_back(eUP);
                dirs.push_back(eRIGHT_UP);
                dirs.push_back(eLEFT_UP);
            }
            else if(piece.GetAlliance() == Alliance::DARK)
            {
                dirs.push_back(eDOWN);
                dirs.push_back(eRIGHT_DOWN);
                dirs.push_back(eLEFT_DOWN);
            }
        }
    }
}
