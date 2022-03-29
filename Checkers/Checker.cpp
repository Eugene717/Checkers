#include "Checker.h"
#include "Game.h"
#include <iostream>

int Checker::NCheckers = 0;
sf::Texture Checker::m_whiteTexture;
sf::Texture Checker::m_blackTexture;
sf::Texture Checker::m_queenTexture;
sf::SoundBuffer Checker::m_fastMoveBuf;
sf::Sound Checker::m_fastMove;
sf::SoundBuffer Checker::m_longMoveBuf;
sf::Sound Checker::m_longMove;
sf::SoundBuffer Checker::m_takeBeatCheckerBuf;
sf::Sound Checker::m_takeBeatChecker;
void Checker::LoadFiles()
{
	if (NCheckers == 0)
	{
		m_whiteTexture.loadFromFile("images/white.png");
		m_blackTexture.loadFromFile("images/black.png");
		m_queenTexture.loadFromFile("images/queen.png");
		m_fastMoveBuf.loadFromFile("sounds/fast_move.wav");
		m_fastMove.setBuffer(m_fastMoveBuf);
		m_longMoveBuf.loadFromFile("sounds/long_move.wav");
		m_longMove.setBuffer(m_longMoveBuf);
		m_takeBeatCheckerBuf.loadFromFile("sounds/take_beat_checker.wav");
		m_takeBeatChecker.setBuffer(m_takeBeatCheckerBuf);
	}
}

Checker::Checker(sf::Vector2i&& pos, char color) noexcept : m_color(color), m_pos(std::move(pos)), m_queen(false)
{
	LoadFiles();
	NCheckers++;

	m_posGraphic.x = m_pos.y * 56 + 56;
	m_posGraphic.y = m_pos.x * 56 + 56;	

	if (m_color == 'w')
		m_sprite.setTexture(m_whiteTexture);
	else
		m_sprite.setTexture(m_blackTexture);
	m_queenSprite.setTexture(m_queenTexture);

	m_sprite.setOrigin(28, 28);
	m_queenSprite.setOrigin(m_queenSprite.getGlobalBounds().width / 2, m_queenSprite.getGlobalBounds().height / 2);

	m_sprite.setPosition(m_posGraphic);
	m_queenSprite.setPosition(m_posGraphic);
}

Checker::~Checker()
{
	if (NCheckers == 0)
	{
		m_whiteTexture.~Texture();
		m_blackTexture.~Texture();
		m_queenTexture.~Texture();
	}
}

bool Checker::operator==(const std::pair<int, int>& other)
{
	return m_pos.x == other.first && m_pos.y == other.second;
}

void Checker::replace(const std::pair<int, int>& pos)
{
	m_pos.x = pos.first;
	m_pos.y = pos.second;

	m_posGraphic.x = m_pos.y * 56 + 56;
	m_posGraphic.y = m_pos.x * 56 + 56;
}

void Checker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
	if (m_queen)
		target.draw(m_queenSprite);
}

sf::FloatRect Checker::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void Checker::setPosition(const float& x, const float& y)
{
	m_sprite.setPosition(x, y);
	m_queenSprite.setPosition(x, y);
}

sf::Vector2i Checker::GetPosition() const
{
	return m_pos;
}	

std::pair<bool, bool> Checker::Move(const sf::Vector2f& posGraphic,const bool& canMoves, bool& queened)
{
	Game* game = Game::GetInstance();
	std::vector<sf::Vector2i> canMove = CanMove();
	
	if (canMoves)
	{
		for (auto i : canMove)
		{
			if (i.x * 56 < posGraphic.y - 28 && i.x * 56 + 56 > posGraphic.y - 28)
			{
				if (i.y * 56 < posGraphic.x - 28 && i.y * 56 + 56 > posGraphic.x - 28)
				{
					setPosition(i.y * 56 + 56, i.x * 56 + 56);
					m_posGraphic = m_sprite.getPosition();

					std::swap(game->m_board[m_pos.x][m_pos.y], game->m_board[i.x][i.y]);

					int hor = -1, vert = -1;
					if (m_pos.x >= i.x + 2)
						vert = i.x + 1;
					else if (m_pos.x <= i.x - 2)
						vert = i.x - 1;
					if (m_pos.y >= i.y + 2)
						hor = i.y + 1;
					else if (m_pos.y <= i.y - 2)
						hor = i.y - 1;

					m_pos = i; 

					bool beated = false;
					if (hor != -1 && vert != -1)
					{
						if (game->m_board[vert][hor] != '\0')
						{
							if (game->GetSound())
								m_takeBeatChecker.play();
							game->m_board[vert][hor] = '\0';	
							beated = true;
						}
					}

					if (game->GetSound())
					{
						if (!beated)
						{
							if (m_queen)
								m_longMove.play();
							else
								m_fastMove.play();
						}
					}

					queened = CheckQueen();					
					
					return std::make_pair<bool, bool>(true, std::move(beated));
				}
			}
		}
	}
	setPosition(m_posGraphic.x, m_posGraphic.y);
	return std::make_pair<bool, bool>(false, false);
}

std::vector<sf::Vector2i> Checker::CanMove(bool* canBeat) const
{
	Game* game = Game::GetInstance();
	std::vector<sf::Vector2i> pos;

	if (m_queen)
	{
		for (int i = m_pos.x + 1, j = m_pos.y + 1; i < 8 && j < 8; i++, j++)  //вниз-вправо
		{
			if (m_color == 'w')
			{
				if (game->m_board[i][j] == 'w')
					break;
				else if (game->m_board[i][j] == 'b')
				{
					if (i < 7 && j < 7)
					{
						if (game->m_board[i + 1][j + 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i + 1, j + 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
			else if (m_color == 'b')
			{
				if (game->m_board[i][j] == 'b')
					break;
				else if (game->m_board[i][j] == 'w')
				{
					if (i < 7 && j < 7)
					{
						if (game->m_board[i + 1][j + 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i + 1, j + 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
		}
		for (int i = m_pos.x + 1, j = m_pos.y - 1; i < 8 && j >= 0; i++, j--)  //вниз-влево
		{
			if (m_color == 'w')
			{
				if (game->m_board[i][j] == 'w')
					break;
				else if (game->m_board[i][j] == 'b')
				{
					if (i < 7 && j > 0)
					{
						if (game->m_board[i + 1][j - 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i + 1, j - 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
			else if (m_color == 'b')
			{
				if (game->m_board[i][j] == 'b')
					break;
				else if (game->m_board[i][j] == 'w')
				{
					if (i < 7 && j > 0)
					{
						if (game->m_board[i + 1][j - 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i + 1, j - 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
		}
		for (int i = m_pos.x - 1, j = m_pos.y + 1; i >= 0 && j < 8; i--, j++)  //вверх-вправо
		{
			if (m_color == 'w')
			{
				if (game->m_board[i][j] == 'w')
					break;
				else if (game->m_board[i][j] == 'b')
				{
					if (i > 0 && j < 7)
					{
						if (game->m_board[i - 1][j + 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i - 1, j + 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
			else if (m_color == 'b')
			{
				if (game->m_board[i][j] == 'b')
					break;
				else if (game->m_board[i][j] == 'w')
				{
					if (i > 0 && j < 7)
					{
						if (game->m_board[i - 1][j + 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i - 1, j + 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
		}
		for (int i = m_pos.x - 1, j = m_pos.y - 1; i >= 0 && j >= 0; i--, j--)  //вверх-влево
		{
			if (m_color == 'w')
			{
				if (game->m_board[i][j] == 'w')
					break;
				else if (game->m_board[i][j] == 'b')
				{
					if (i > 0 && j > 0)
					{
						if (game->m_board[i - 1][j - 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i - 1, j - 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
			else if (m_color == 'b')
			{
				if (game->m_board[i][j] == 'b')
					break;
				else if (game->m_board[i][j] == 'w')
				{
					if (i > 0 && j > 0)
					{
						if (game->m_board[i - 1][j - 1] == '\0')
						{
							pos.push_back(sf::Vector2i(i - 1, j - 1));
							canBeat == nullptr ? NULL : *canBeat = true;
							break;
						}
					}
					break;
				}
			}
		}
		if (pos.empty())
		{
			for (int i = m_pos.x + 1, j = m_pos.y + 1; i < 8 && j < 8; i++, j++)  //вниз-вправо
			{
				if (game->m_board[i][j] == m_color)
					break;
				if (game->m_board[i][j] == '\0')
				{
					pos.push_back(sf::Vector2i(i, j));
				}
				else
					break;
			}
			for (int i = m_pos.x + 1, j = m_pos.y - 1; i < 8 && j >= 0; i++, j--)  //вниз-влево
			{
				if (game->m_board[i][j] == m_color)
					break;
				if (game->m_board[i][j] == '\0')
				{
					pos.push_back(sf::Vector2i(i, j));
				}
				else
					break;
			}
			for (int i = m_pos.x - 1, j = m_pos.y + 1; i >= 0 && j < 8; i--, j++)  //вверх-вправо
			{
				if (game->m_board[i][j] == m_color)
					break;
				if (game->m_board[i][j] == '\0')
				{
					pos.push_back(sf::Vector2i(i, j));
				}
				else
					break;
			}
			for (int i = m_pos.x - 1, j = m_pos.y - 1; i >= 0 && j >= 0; i--, j--)  //вверх-влево
			{
				if (game->m_board[i][j] == m_color)
					break;
				if (game->m_board[i][j] == '\0')
				{
					pos.push_back(sf::Vector2i(i, j));
				}
				else
					break;
			}
		}
	}
	else //!m_queen
	{
		if (m_color == 'w')
		{
			if (game->m_board[m_pos.x + 1][m_pos.y + 1] == 'b')  //бить назад
			{
				if (m_pos.x < 6 && m_pos.y < 6)
					if (game->m_board[m_pos.x + 2][m_pos.y + 2] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x + 2, m_pos.y + 2));
						canBeat == nullptr ? NULL : *canBeat = true;
					}
			}
			if (game->m_board[m_pos.x + 1][m_pos.y - 1] == 'b')  //бить назад
			{
				if (m_pos.x < 5 && m_pos.y>1)
					if (game->m_board[m_pos.x + 2][m_pos.y - 2] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x + 2, m_pos.y - 2));
						canBeat == nullptr ? NULL : *canBeat = true;
					}
			}
			if (m_pos.x > 0 && m_pos.y > 0)
			{				
				if (game->m_board[m_pos.x - 1][m_pos.y - 1] == 'b') //another color
				{
					if (m_pos.x >= 2 && m_pos.y >= 2)
						if (game->m_board[m_pos.x - 2][m_pos.y - 2] == '\0')  //бить вперед
						{
							pos.push_back(sf::Vector2i(m_pos.x - 2, m_pos.y - 2));
							canBeat == nullptr ? NULL : *canBeat = true;
						}
				}
			}
			if (m_pos.x > 0 && m_pos.y < 7)
			{				
				if (game->m_board[m_pos.x - 1][m_pos.y + 1] == 'b')  //another color
				{
					if (m_pos.x >= 2 && m_pos.y <= 5)
						if (game->m_board[m_pos.x - 2][m_pos.y + 2] == '\0')  //бить вперед
						{
							pos.push_back(sf::Vector2i(m_pos.x - 2, m_pos.y + 2));
							canBeat == nullptr ? NULL : *canBeat = true;
						}
				}
			}
			if (pos.empty())  //если не может бить, можем ходить
			{
				if (m_pos.x > 0 && m_pos.y > 0)
					if (game->m_board[m_pos.x - 1][m_pos.y - 1] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x - 1, m_pos.y - 1));
					}
				if (m_pos.x > 0 && m_pos.y < 7)
					if (game->m_board[m_pos.x - 1][m_pos.y + 1] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x - 1, m_pos.y + 1));
					}
			}
		}
		else  //m_color == 'b'
		{
			if (game->m_board[m_pos.x - 1][m_pos.y - 1] == 'w') //бить назад
			{
				if (m_pos.x >= 2 && m_pos.y >= 2)
					if (game->m_board[m_pos.x - 2][m_pos.y - 2] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x - 2, m_pos.y - 2));
						canBeat == nullptr ? NULL : *canBeat = true;
					}
			}
			if (game->m_board[m_pos.x - 1][m_pos.y + 1] == 'w')  //бить назад
			{
				if (m_pos.x >= 2 && m_pos.y <= 5)
					if (game->m_board[m_pos.x - 2][m_pos.y + 2] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x - 2, m_pos.y + 2));
						canBeat == nullptr ? NULL : *canBeat = true;
					}
			}
			if (m_pos.x < 7 && m_pos.y < 7)
			{
				if (game->m_board[m_pos.x + 1][m_pos.y + 1] == 'w')  //бить вперед
				{
					if (m_pos.x < 6 && m_pos.y < 6)
						if (game->m_board[m_pos.x + 2][m_pos.y + 2] == '\0')
						{
							pos.push_back(sf::Vector2i(m_pos.x + 2, m_pos.y + 2));
							canBeat == nullptr ? NULL : *canBeat = true;
						}
				}
			}
			if (m_pos.x < 7 && m_pos.y > 0)
			{				
				if (game->m_board[m_pos.x + 1][m_pos.y - 1] == 'w')  //бить вперед
				{
					if (m_pos.x < 6 && m_pos.y > 1)
						if (game->m_board[m_pos.x + 2][m_pos.y - 2] == '\0')
						{
							pos.push_back(sf::Vector2i(m_pos.x + 2, m_pos.y - 2));
							canBeat == nullptr ? NULL : *canBeat = true;
						}
				}
			}
			if (pos.empty())  //если не может бить, можем ходить
			{
				if (m_pos.x < 7 && m_pos.y < 7)
					if (game->m_board[m_pos.x + 1][m_pos.y + 1] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x + 1, m_pos.y + 1));
					}
				if (m_pos.x < 7 && m_pos.y > 0)
					if (game->m_board[m_pos.x + 1][m_pos.y - 1] == '\0')
					{
						pos.push_back(sf::Vector2i(m_pos.x + 1, m_pos.y - 1));
					}
			}
		}
	}

	return pos;
}

bool Checker::Alived() const
{
	Game* game = Game::GetInstance();
	if (game->m_board[m_pos.x][m_pos.y] == m_color)
	{
		return true;
	}
	else
	{
		NCheckers--;
		return false;
	}
}

bool Checker::CheckQueen()
{
	if (!m_queen)
	{
		if (m_color == 'b')
		{
			if (m_pos.x == 7)
			{
				m_queen = true;
				return true;
			}
		}
		else if (m_color == 'w')
		{
			if (m_pos.x == 0)
			{
				m_queen = true;
				return true;
			}
		}
	}
	return false;
}
