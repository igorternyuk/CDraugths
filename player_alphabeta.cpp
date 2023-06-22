#include "player_alphabeta.hpp"
#include "alphabeta.hpp"
#include "euristic_default.hpp"

PlayerAlphaBeta::PlayerAlphaBeta(Alliance alliance, int depth) :
    PlayerCPU(alliance, std::make_shared<MiniMaxAlphaBeta>(std::make_shared<EuristicDefault>(),depth))
{

}
