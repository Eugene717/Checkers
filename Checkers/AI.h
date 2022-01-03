#pragma once
#include "Player.h"
class AI :
    public Player
{

public:
    AI(char&& color);
    bool MakeMove() override;
};

