#pragma once
#include "game.hpp"

class View
{
public:

private:
    explicit View();
    View(const View& game) = delete;
    View(View&& game) = delete;
    View& operator=(const View& game) = delete;
    View& operator=(View&& game) = delete;
private:

};
