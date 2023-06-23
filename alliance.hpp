#pragma once

namespace draughts
{
    enum class Alliance
    {
        RED,
        BLUE,
        NONE
    };

    Alliance OpponentAlliance(Alliance& alliance);
    int DirectionOfAlliance(Alliance& alliance);
}
