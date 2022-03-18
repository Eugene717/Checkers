#include "Human.h"
#include "Game.h"

Human::Human(char&& color) :Player(std::move(color))
{ }

void Human::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Game* game = Game::GetInstance();

	for (auto i : m_checkers)
	{
		if (m_p_moved_checker != &i)
			game->m_window.draw(i);
	}
	if (m_p_moved_checker != nullptr)
		game->m_window.draw(*m_p_moved_checker);
}

std::vector<sf::Vector2i> Human::GetCanBeatNPos() const
{
	std::vector<sf::Vector2i> pos;

	for (auto i : m_canBeatN)
	{
		pos.push_back(m_checkers[i].GetPosition());
	}

	return pos;
}

bool Human::MakeMove()
{
	Game* game = Game::GetInstance();
	CanBeatN();
	bool isMove = false;
	sf::Vector2f pos;
	int nChecker = -1;

	while (game->m_window.isOpen())
	{
		pos = game->m_window.mapPixelToCoords(sf::Mouse::getPosition(game->m_window));

		if (CanBeat() && !isMove)
		{
			game->DrawPossibleBlows(GetCanBeatNPos());
		}

		if (game->m_window.pollEvent(game->m_event))
		{
			if (game->m_event.type == sf::Event::Closed)
				game->m_window.close();
			if (game->m_event.type == sf::Event::KeyReleased)
			{
				if (game->m_event.key.code == sf::Keyboard::Escape)
				{
					if (game->DrawMenu())
						return false;
				}
			}
			if (game->m_event.type == sf::Event::MouseButtonPressed)
			{
				if (game->m_event.mouseButton.button == sf::Mouse::Left)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						for (int i = 0; i < m_checkers.size(); i++)
						{
							if (sf::IntRect(m_checkers[i].getGlobalBounds()).contains(sf::Mouse::getPosition(game->m_window)))
							{
								isMove = true;
								nChecker = i;
								m_p_moved_checker = &m_checkers[i];
							}
						}
					}
				}
			}
			if (game->m_event.type == sf::Event::MouseButtonReleased)
			{
				if (game->m_event.key.code == sf::Mouse::Left)
				{
					if (isMove)
					{
						bool canMove = true;
						if (!m_canBeatN.empty())
						{
							for (int i = 0; i < m_canBeatN.size(); i++)
							{
								if (!std::binary_search(m_canBeatN.begin(), m_canBeatN.end(), nChecker))
								{
									canMove = false;
									m_p_moved_checker = nullptr;
									break;
								}
							}
						}
						game->DrawGame();
						std::pair<bool, bool> moveRes = m_checkers[nChecker].Move(sf::Vector2f(pos.x, pos.y), canMove, queened);
						if (moveRes.first)
						{
							m_NbeatChecker = nChecker;
							game->DrawGame();
							m_p_moved_checker = nullptr;
							return moveRes.second;
						}
						game->DrawGame();
						isMove = false;
					}
				}
			}
		}
		if (isMove)
		{
			m_checkers[nChecker].setPosition(pos.x, pos.y);
			bool canMove = true;
			if (!m_canBeatN.empty())
			{
				for (int i = 0; i < m_canBeatN.size(); i++)
				{
					if (!std::binary_search(m_canBeatN.begin(),m_canBeatN.end(), nChecker))
					{
						canMove = false;
						m_p_moved_checker = nullptr;
						break;
					}
				}
			}
			game->DrawPossibleMoves(m_checkers[nChecker].CanMove(), canMove, GetColor());
		}
	}
}
