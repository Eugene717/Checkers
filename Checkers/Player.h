#pragma once
#include <vector>
#include "Checker.h"

class Player :public sf::Drawable
{
	char m_color;
	std::string m_name;
protected:
	std::vector<Checker> m_checkers;
	std::vector<int> m_canBeatN;
	int m_NbeatChecker;
	bool m_queened;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Player(char&& color);
	~Player();

	void SetName(const std::string& name);
	std::string GetName() const;
	bool EatChecker();
	void CanBeatN();
	bool CanBeat();
	bool CanBeatAgain();
	char GetColor() const;
	virtual bool MakeMove() = 0;
};
