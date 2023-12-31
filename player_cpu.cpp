#include "player_cpu.h"

using namespace draughts;

PlayerCPU::PlayerCPU(Alliance alliance, std::shared_ptr<MoveStrategy> strategy):
        Player(alliance), _moveStrategy(strategy)
{

}

Move PlayerCPU::MakeMove(const Position &position)
{
    Move bestMove;
    if(!position.IsEndgameScenario() && _moveStrategy)
    {
        std::shared_ptr<Position> positionCopy = position.MakeCopy();
        bestMove = _moveStrategy->GetBestMove(positionCopy, *this);
        bestMove.UpdateStatus();
    }

    return bestMove;
}

bool PlayerCPU::IsHuman() const
{
    return false;
}
