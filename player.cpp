#include "player.h"
#include "alliance.h"

using namespace draughts;

Player::Player(Alliance alliance): _alliance(alliance)
{

}

Alliance Player::GetAlliance() const
{
    return _alliance;
}

Alliance Player::GetOpponentAlliance() const
{
    if(_alliance == Alliance::NONE)
        return Alliance::NONE;
    return _alliance == Alliance::DARK ? Alliance::LIGHT : Alliance::DARK;
}
