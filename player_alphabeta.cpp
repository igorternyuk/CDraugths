#include "player_alphabeta.h"
#include "alphabeta.h"
#include "heuristic_default.h"

using namespace draughts;

PlayerAlphaBeta::PlayerAlphaBeta(Alliance alliance, int depth, std::shared_ptr<EvaluationStrategy> evaluationStrategy) :
        PlayerCPU(alliance, std::make_shared<MiniMaxAlphaBeta>(evaluationStrategy, depth))
{

}
