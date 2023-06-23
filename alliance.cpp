#include "alliance.hpp"

draughts::Alliance draughts::OpponentAlliance(Alliance &alliance)
{
    if(alliance == Alliance::NONE)
        return Alliance::NONE;
    return alliance == Alliance::RED ? Alliance::BLUE : Alliance::RED;
}

int draughts::DirectionOfAlliance(Alliance &alliance)
{
    if(alliance == Alliance::NONE)
        return 0;
    return alliance == Alliance::RED ? -1 : 1;
}
