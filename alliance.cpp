#include "alliance.h"

draughts::Alliance draughts::OpponentAlliance(Alliance &alliance)
{
    if(alliance == Alliance::NONE)
        return Alliance::NONE;
    return alliance == Alliance::DARK ? Alliance::LIGHT : Alliance::DARK;
}

int draughts::DirectionOfAlliance(Alliance &alliance)
{
    if(alliance == Alliance::NONE)
        return 0;
    return alliance == Alliance::DARK ? 1 : -1;
}
