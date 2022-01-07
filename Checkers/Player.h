#pragma once
#include <vector>
#include "Checker.h"

class Player :public sf::Drawable
{
	char m_color;
protected:
	std::vector<Checker> m_checkers;
	std::vector<int> m_canBeatN;
	int m_NbeatChecker;
	bool queened;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Player(char&& color);
	~Player();

	bool EatChecker();
	void CanBeatN();
	bool CanBeat();
	bool CanBeatAgain();
	char GetColor() const;
	virtual bool MakeMove() = 0;
};
