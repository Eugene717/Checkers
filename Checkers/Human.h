#pragma once
#include "Player.h"

class Human :
    public Player
{

public:
    Human(char&& color);
    bool MakeMove() override;
};

