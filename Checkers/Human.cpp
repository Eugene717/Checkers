#include "Human.h"
#include "Game.h"

Human::Human(char&& color) :Player(std::move(color))
{ }

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

		if (game->m_window.pollEvent(game->m_event))
		{
			if (game->m_event.type == sf::Event::Closed)
				game->m_window.close();

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
									break;
								}
							}
						}
						game->DrawGame();
						std::pair<bool, bool> moveRes = m_checkers[nChecker].Move(sf::Vector2f(pos.x, pos.y), canMove);
						if (moveRes.first)
						{
							game->DrawGame();
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
						break;
					}
				}
			}
			game->DrawPossibleMoves(m_checkers[nChecker].CanMove(), canMove, GetColor());
		}
	}
}
