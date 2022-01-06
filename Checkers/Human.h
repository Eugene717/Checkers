#pragma once
#include "Player.h"

class Human :
    public Player
{
    std::vector<sf::Vector2i> GetCanBeatNPos() const;
public:
    Human(char&& color);
    bool MakeMove() override;
};

