#include "Player.h"
#include "Game.h"

Player::Player(char&& color) :m_color(std::move(color))
{
	Game* game = Game::GetInstance();
	m_NbeatChecker = -1;
	m_queened = false;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (game->m_board[i][j] == m_color)
			{
				m_checkers.push_back(Checker(sf::Vector2i(i, j), game->m_board[i][j]));
			}
		}
	}
}

Player::~Player()
{
	m_checkers.clear();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Game* game = Game::GetInstance();

	for (auto i : m_checkers)
	{
		game->m_window.draw(i);
	}
}

void Player::SetName(const std::string& name)
{
	m_name = name;
}

std::string Player::GetName() const
{
	return m_name;
}

bool Player::EatChecker()
{
	m_checkers.erase(std::remove_if(m_checkers.begin(), m_checkers.end(), [&](Checker& i) { if (i.Alived()) return false; else return true; }), m_checkers.end());

	bool canMove = false;

	for (size_t i = 0; i < m_checkers.size(); i++)
	{
		if (m_checkers[i].CanMove().size() > 0)
		{
			canMove = true;
			break;
		}
	}

	if (m_checkers.empty() || !canMove)
	{
		Game* game = Game::GetInstance();
		game->m_dataPacket.m_finishGame = true;
		return true;
	}
	else
		return false;
}

void Player::CanBeatN()
{
	m_canBeatN.clear();

	for (int i = 0; i < m_checkers.size(); i++)
	{
		bool canBeat = false;
		m_checkers[i].CanMove(&canBeat);	
		if (canBeat)
			m_canBeatN.push_back(i);
	}
}

bool Player::CanBeat()
{
	CanBeatN();
	if (m_canBeatN.empty())
		return false;
	else
		return true;
}

bool Player::CanBeatAgain()
{
	if (m_queened == true)
	{
		m_queened = false;
		return false;
	}

	CanBeatN();

	for (auto i : m_canBeatN)
	{
		if (m_NbeatChecker == i)
			return true;
	}
	return false;
}

char Player::GetColor() const
{
	return m_color;
}