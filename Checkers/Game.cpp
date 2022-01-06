#include "Game.h"
#include "Player.h"
#include "Human.h"
#include "AI.h"
#include <random>

Game* Game::m_game = nullptr;

struct GameIMPL
{
	sf::Font m_font;
	sf::Texture m_t_board;
	sf::Sprite m_s_board;
	Player* m_playerOne;
	Player* m_playerTwo;
	std::default_random_engine m_random;

	GameIMPL();
};

GameIMPL::GameIMPL()
{
	m_font.loadFromFile("arial.ttf");
	m_t_board.loadFromFile("images/board.png");
	m_s_board.setTexture(m_t_board); 
	std::random_device rd;
	m_random.seed(rd());
}

Game::Game()
{
	m_pImpl = new GameIMPL;
	m_pImpl->m_font.loadFromFile("arial.ttf");
	m_window.create(sf::VideoMode(504,504), "Checkers");
	SetStartedBoard();
}

Game* Game::GetInstance()
{
	if (m_game == nullptr)
		m_game = new Game();
	return m_game;
}

Game::~Game()
{
	delete m_game;
	delete m_pImpl;
}

char Game::FirstTurn() const
{
	if (m_pImpl->m_random() % 2 == 0)
		return 'F';
	else
		return 'S';
}

void Game::SetStartedBoard()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_board[i][j] = '\0';
		}
	}

	m_board[0][1] = 'b';m_board[0][3] = 'b';m_board[0][5] = 'b';m_board[0][7] = 'b';
	m_board[1][0] = 'b';m_board[1][2] = 'b';m_board[1][4] = 'b';m_board[1][6] = 'b';
	m_board[2][1] = 'b';m_board[2][3] = 'b';m_board[2][5] = 'b';m_board[2][7] = 'b';

	m_board[5][0] = 'w';m_board[5][2] = 'w';m_board[5][4] = 'w';m_board[5][6] = 'w';
	m_board[6][1] = 'w';m_board[6][3] = 'w';m_board[6][5] = 'w';m_board[6][7] = 'w';
	m_board[7][0] = 'w';m_board[7][2] = 'w';m_board[7][4] = 'w';m_board[7][6] = 'w';
}

int Game::MainMenu()
{
	float centerPos = m_window.getSize().x / 2;

	sf::Text header("Checkers", m_pImpl->m_font);
	header.setCharacterSize(72);
	header.setStyle(sf::Text::Bold);
	header.setPosition(centerPos - header.getGlobalBounds().width / 2, 0);

	sf::Text singleplayer("Singlelayer", m_pImpl->m_font);
	singleplayer.setPosition(centerPos - singleplayer.getGlobalBounds().width / 2, header.getPosition().y + 150);

	sf::Text onePC("Two on one PC", m_pImpl->m_font);
	onePC.setPosition(centerPos - onePC.getGlobalBounds().width / 2, singleplayer.getPosition().y + 70);

	sf::Text multiplayer("Multiplayer", m_pImpl->m_font);
	multiplayer.setPosition(centerPos - multiplayer.getGlobalBounds().width / 2, onePC.getPosition().y + 70);

	sf::Text settings("Settings", m_pImpl->m_font);
	settings.setPosition(centerPos - settings.getGlobalBounds().width / 2, multiplayer.getPosition().y + 70);

	sf::Text exit("Exit", m_pImpl->m_font);
	exit.setPosition(centerPos - exit.getGlobalBounds().width / 2, settings.getPosition().y + 70);

	int menuNum;

	while (m_window.isOpen())
	{
		while (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
			if (m_event.type == sf::Event::KeyPressed)
			{
				if (m_event.key.code == sf::Keyboard::Escape)
					m_window.close();
			}
		}

		menuNum = 0;

		header.setFillColor(sf::Color::Black);
		singleplayer.setFillColor(sf::Color::Black);
		onePC.setFillColor(sf::Color::Black);
		multiplayer.setFillColor(sf::Color::Black);
		settings.setFillColor(sf::Color::Black);
		exit.setFillColor(sf::Color::Black);

		if (sf::IntRect(singleplayer.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			singleplayer.setFillColor(sf::Color::Blue);
			menuNum = 1;
		}
		if (sf::IntRect(onePC.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			onePC.setFillColor(sf::Color::Blue);
			menuNum = 2;
		}
		if (sf::IntRect(multiplayer.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			multiplayer.setFillColor(sf::Color::Blue);
			menuNum = 3;
		}
		if (sf::IntRect(settings.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			settings.setFillColor(sf::Color::Blue);
			menuNum = 4;
		}
		if (sf::IntRect(exit.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			exit.setFillColor(sf::Color::Blue);
			menuNum = 5;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && menuNum > 0)
		{
			m_window.clear(sf::Color::White);
			return menuNum;
		}

		m_window.clear(sf::Color::White);
		m_window.draw(header);
		m_window.draw(singleplayer);
		m_window.draw(onePC);
		m_window.draw(multiplayer);
		m_window.draw(settings);
		m_window.draw(exit);
		m_window.display();
	}
	return -1;
}

void Game::DrawGame()
{
	m_window.clear(sf::Color::White);

	m_window.draw(m_pImpl->m_s_board);
	m_window.draw(*m_pImpl->m_playerOne);
	m_window.draw(*m_pImpl->m_playerTwo);
	m_window.display();
}

void Game::DrawPossibleMoves(const std::vector<sf::Vector2i>& pos, const bool& canMove, const char& player)
{
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color(255, 255, 0, 0));
	shape.setOutlineColor(sf::Color(255, 255, 0));
	shape.setOutlineThickness(2);
	shape.setSize(sf::Vector2f(56, 56));

	m_window.draw(m_pImpl->m_s_board);

	if (canMove)
	{
		for (auto i : pos)
		{
			shape.setPosition(i.y * 56 + 28, i.x * 56 + 28);
			m_window.draw(shape);
		}
	}

	if (player == 'w')
	{
		m_window.draw(*m_pImpl->m_playerOne);
		m_window.draw(*m_pImpl->m_playerTwo);
	}
	else
	{
		m_window.draw(*m_pImpl->m_playerTwo);
		m_window.draw(*m_pImpl->m_playerOne);
	}
	m_window.display();
}

void Game::DrawPossibleBlows(const std::vector<sf::Vector2i>& pos)
{	
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color(255, 255, 0, 0));
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(2);
	shape.setSize(sf::Vector2f(56, 56));

	m_window.draw(m_pImpl->m_s_board);

	for (auto i : pos)
	{
		shape.setPosition(i.y * 56 + 28, i.x * 56 + 28);
		m_window.draw(shape);
	}

	m_window.draw(*m_pImpl->m_playerOne);
	m_window.draw(*m_pImpl->m_playerTwo);

	m_window.display();
}

void Game::AnnounceWinner(const char& color)
{
	sf::sleep(sf::seconds(1));
	sf::Vector2f centerPos = sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2);

	sf::Text announce("",m_pImpl->m_font);
	announce.setFillColor(sf::Color::Black);
	announce.setCharacterSize(30);
	announce.setStyle(sf::Text::Style::Bold);

	if (color == 'w')
	{
		announce.setString("WHITE PLAYER WIN!");
		announce.setPosition(centerPos.x - announce.getGlobalBounds().width / 2, centerPos.y - announce.getGlobalBounds().height / 2 - 100);
	}
	else if (color == 'b')
	{
		announce.setString("BLACK PLAYER WIN!");
		announce.setPosition(centerPos.x - announce.getGlobalBounds().width / 2, centerPos.y - announce.getGlobalBounds().height / 2 - 100);
	}

	sf::sleep(sf::seconds(2));
	m_window.clear(sf::Color::White);
	m_window.draw(announce);
	m_window.display();
	sf::sleep(sf::seconds(3));

	SetStartedBoard();
	delete m_pImpl->m_playerOne;
	delete m_pImpl->m_playerTwo;
	m_pImpl->m_playerOne = nullptr;
	m_pImpl->m_playerTwo = nullptr;
}

bool Game::Exit()
{
	sf::Vector2f centerPos = sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 100);

	sf::Text exit("Are you sure about that?", m_pImpl->m_font);
	exit.setFillColor(sf::Color::Black);
	exit.setCharacterSize(30);
	exit.setStyle(sf::Text::Style::Bold);
	exit.setPosition(centerPos.x - exit.getGlobalBounds().width / 2, centerPos.y - exit.getGlobalBounds().height);

	sf::Text yes("Yes", m_pImpl->m_font);
	sf::Text no("No", m_pImpl->m_font);

	yes.setPosition(centerPos.x - yes.getGlobalBounds().width / 2 - 50, centerPos.y - yes.getGlobalBounds().height / 2 + 50);
	no.setPosition(centerPos.x - no.getGlobalBounds().width / 2 + 50, centerPos.y - no.getGlobalBounds().height / 2 + 50);

	while (m_window.isOpen())
	{
		while (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
			if (m_event.type == sf::Event::KeyPressed)
			{
				if (m_event.key.code == sf::Keyboard::Escape)
					m_window.close();
			}
			if (m_event.type == m_event.MouseButtonReleased && m_event.mouseButton.button == sf::Mouse::Left)
			{
				if (sf::IntRect(yes.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					return true;
				if (sf::IntRect(no.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_window.clear(sf::Color::White);
					return false;
				}
			}
		}

		yes.setFillColor(sf::Color::Black);
		no.setFillColor(sf::Color::Black);

		if (sf::IntRect(yes.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			yes.setFillColor(sf::Color::Blue);
		}
		if (sf::IntRect(no.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			no.setFillColor(sf::Color::Blue);
		}

		m_window.clear(sf::Color::White);
		m_window.draw(exit);
		m_window.draw(yes);
		m_window.draw(no);
		m_window.display();
	}
	return true;
}

void Game::SinglePlayer()
{
	m_pImpl->m_playerOne = new Human('b');
	m_pImpl->m_playerTwo = new AI('w');

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
		}

		

	}
}

void Game::OnePC()
{
	m_pImpl->m_playerOne = new Human('b');
	m_pImpl->m_playerTwo = new Human('w');

	char turn = FirstTurn();

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
		}

		DrawGame();

		if (turn == 'F')
		{
			while (m_pImpl->m_playerOne->MakeMove())
			{
				if (!m_pImpl->m_playerTwo->EatChecker())
				{
					DrawGame();
					if (!m_pImpl->m_playerOne->CanBeatAgain())
						break;
				}
				else
				{
					DrawGame();
					AnnounceWinner('b');
					return;
				}
			}
			while (m_pImpl->m_playerTwo->MakeMove())
			{
				if (!m_pImpl->m_playerOne->EatChecker())
				{
					DrawGame();
					if (!m_pImpl->m_playerTwo->CanBeatAgain())
						break;
				}
				else
				{
					DrawGame();
					AnnounceWinner('w');
					return;
				}
			}				
		}
		else  //'S'
		{
			while (m_pImpl->m_playerTwo->MakeMove())
			{
				if (!m_pImpl->m_playerOne->EatChecker())
				{
					DrawGame();
					if (!m_pImpl->m_playerTwo->CanBeatAgain())
						break;
				}
				else
				{
					DrawGame();
					AnnounceWinner('w');
					return;
				}
			}
			while (m_pImpl->m_playerOne->MakeMove())
			{
				if (!m_pImpl->m_playerTwo->EatChecker())
				{
					DrawGame();
					if (!m_pImpl->m_playerOne->CanBeatAgain())
						break;
				}
				else
				{
					DrawGame();
					AnnounceWinner('b');
					return;
				}
			}
		}

	}
}

void Game::Multiplayer()
{

}
