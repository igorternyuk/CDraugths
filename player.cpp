#include "player.hpp"
#include "alliance.hpp"

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
    return _alliance == Alliance::RED ? Alliance::BLUE : Alliance::RED;
}
