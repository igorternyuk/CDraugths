#pragma once

namespace draughts
{
    enum class Alliance
    {
        DARK,
        LIGHT,
        NONE
    };

    Alliance OpponentAlliance(Alliance& alliance);
    int DirectionOfAlliance(Alliance& alliance);
}
