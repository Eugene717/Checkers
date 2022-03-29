#pragma once
#include "Player.h"

class Human :
    public Player
{
    std::vector<sf::Vector2i> GetCanBeatNPos() const;
    Checker* m_p_moved_checker;
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    Human(char&& color);
    bool MakeMove() override;
    bool MoveMP();
};

