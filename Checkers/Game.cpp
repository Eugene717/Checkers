#include "Game.h"
#include "Player.h"
#include "Human.h"
#include "AI.h"

Game* Game::m_game = nullptr;

struct GameIMPL
{
	sf::Font m_font;
	sf::Texture m_t_board;
	sf::Sprite m_s_board;
	bool m_sound;
	bool m_returnToMainMenu;
	bool m_MP;
	sf::Texture m_t_soundOn;
	sf::Texture m_t_soundOff;
	sf::Sprite m_s_sound;
	Player* m_playerOne;
	Player* m_playerTwo;

	GameIMPL();
};

GameIMPL::GameIMPL()
{
	m_font.loadFromFile("arial.ttf");
	m_t_board.loadFromFile("images/board.png");
	m_s_board.setTexture(m_t_board); 
	m_t_soundOn.loadFromFile("images/sound_on.png");	
	m_t_soundOff.loadFromFile("images/sound_off.png");	
	m_sound = true;
	m_returnToMainMenu = false;
	m_MP = false;
	
	m_playerOne = nullptr;
	m_playerTwo = nullptr;
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

bool Game::GetSound() const
{
	return m_pImpl->m_sound;
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
			if (m_event.type == sf::Event::MouseButtonReleased)
			{
				if (m_event.key.code == sf::Mouse::Left)
				{
					m_window.clear(sf::Color::White);
					return menuNum;
				}
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

bool Game::DrawMenu()
{
	float centerPos = m_window.getSize().x / 2;

	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	shape.setSize(sf::Vector2f(300, 150));
	shape.setOrigin(150, 75);
	shape.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 2);

	sf::Text toMenu("Exit to menu", m_pImpl->m_font);
	toMenu.setCharacterSize(40);
	toMenu.setFillColor(sf::Color::Black);
	toMenu.setOrigin(toMenu.getGlobalBounds().width / 2, toMenu.getGlobalBounds().height / 2);
	toMenu.setPosition(252, 190);
	sf::Text exit("Exit to desktop", m_pImpl->m_font);
	exit.setFillColor(sf::Color::Black);
	exit.setCharacterSize(40);
	exit.setOrigin(exit.getGlobalBounds().width / 2, exit.getGlobalBounds().height / 2);
	exit.setPosition(252, toMenu.getPosition().y + 60);
	if (m_pImpl->m_sound)
		m_pImpl->m_s_sound.setTexture(m_pImpl->m_t_soundOn);
	else
		m_pImpl->m_s_sound.setTexture(m_pImpl->m_t_soundOff);
	m_pImpl->m_s_sound.setPosition(shape.getGlobalBounds().left + 284, shape.getGlobalBounds().top + 134);
	m_pImpl->m_s_sound.setOrigin(16, 16);

	sf::Vector2f pos;
	int menu; 

	while (m_window.isOpen())
	{
		while (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
			if (m_event.type == sf::Event::KeyReleased)
			{
				if (m_event.key.code == sf::Keyboard::Escape)
				{
					DrawGame();
					return false;
				}			
			}
			if (m_event.type == sf::Event::MouseButtonReleased && m_event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(m_pImpl->m_s_sound.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					if (m_pImpl->m_sound)
					{
						m_pImpl->m_sound = false;
						m_pImpl->m_s_sound.setTexture(m_pImpl->m_t_soundOff);
					}
					else
					{
						m_pImpl->m_sound = true;
						m_pImpl->m_s_sound.setTexture(m_pImpl->m_t_soundOn);
					}
				}
			}
		}

		pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
		menu = -1;

		toMenu.setFillColor(sf::Color::Black);
		exit.setFillColor(sf::Color::Black);
		m_pImpl->m_s_sound.setScale(1, 1);
		toMenu.setScale(1, 1);
		exit.setScale(1, 1);

		if (sf::IntRect(toMenu.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			toMenu.setScale(1.05, 1.05);
			if (m_event.type == sf::Event::MouseButtonReleased)
			{
				if (m_event.key.code == sf::Mouse::Left)
				{
					delete m_pImpl->m_playerOne;
					m_pImpl->m_playerOne = nullptr;
					delete m_pImpl->m_playerTwo;
					m_pImpl->m_playerTwo = nullptr;
					m_pImpl->m_returnToMainMenu = true;
					return true;
				}
			}
		}
		if (sf::IntRect(exit.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			exit.setScale(1.05, 1.05);
			if (m_event.type == sf::Event::MouseButtonReleased)
			{
				if (m_event.key.code == sf::Mouse::Left)
				{
					delete m_pImpl->m_playerOne;
					delete m_pImpl->m_playerTwo;
					m_window.close();
				}
			}
		}
		if (sf::IntRect(m_pImpl->m_s_sound.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			m_pImpl->m_s_sound.setScale(1.15, 1.15);
			
		}		
		if (!sf::IntRect(shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			if (m_event.type == sf::Event::MouseButtonReleased)
			{
				if (m_event.key.code == sf::Mouse::Left)
				{
					DrawGame();
					return false;
				}
			}
		}

		m_window.draw(m_pImpl->m_s_board);
		m_window.draw(*m_pImpl->m_playerOne);
		m_window.draw(*m_pImpl->m_playerTwo);

		m_window.draw(shape);
		m_window.draw(toMenu);
		m_window.draw(exit);
		m_window.draw(m_pImpl->m_s_sound);
		m_window.display();
	}
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

	if (player == 'w' || m_pImpl->m_MP)
	{
		m_window.draw(*m_pImpl->m_playerTwo);
		m_window.draw(*m_pImpl->m_playerOne);
	}
	else
	{
		m_window.draw(*m_pImpl->m_playerOne);
		m_window.draw(*m_pImpl->m_playerTwo);
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

void Game::AnnounceWinner(const char& color, const std::string name = "\0")
{
	sf::sleep(sf::seconds(1));
	sf::Vector2f centerPos = sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2);

	sf::Text announce("",m_pImpl->m_font);
	announce.setFillColor(sf::Color::Black);
	announce.setCharacterSize(30);
	announce.setStyle(sf::Text::Style::Bold);

	if (name != "\0")
	{
		std::string sColor;
		color == 'w' ? sColor = " (white) " : sColor = " (black) ";
		announce.setString(name + sColor + "WIN!");
	}
	else if (color == 'w')
	{
		announce.setString("WHITE PLAYER WIN!");
	}
	else if (color == 'b')
	{
		announce.setString("BLACK PLAYER WIN!");
	}

	announce.setPosition(centerPos.x - announce.getGlobalBounds().width / 2, centerPos.y - announce.getGlobalBounds().height / 2 - 100);

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

		DrawGame();

	}
}

void Game::OnePC()
{
	m_pImpl->m_playerOne = new Human('w');
	m_pImpl->m_playerTwo = new Human('b');

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
		}

		DrawGame();

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
				AnnounceWinner('w');
				return;
			}
		}
		if (m_pImpl->m_returnToMainMenu)
		{
			m_pImpl->m_returnToMainMenu = false;
			SetStartedBoard();
			return;
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
				AnnounceWinner('b');
				return;
			}
		}
		if (m_pImpl->m_returnToMainMenu)
		{
			m_pImpl->m_returnToMainMenu = false;
			SetStartedBoard();
			return;
		}

	}
}

std::string Game::EnterName()
{
	sf::Vector2f centerPos = sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 100);

	sf::Text enter("Enter your name", m_pImpl->m_font);
	enter.setFillColor(sf::Color::Black);
	enter.setCharacterSize(30);
	enter.setPosition(centerPos.x - enter.getGlobalBounds().width / 2, centerPos.y - enter.getGlobalBounds().height);

	sf::Text back("Back", m_pImpl->m_font);
	back.setFillColor(sf::Color::Black);
	back.setCharacterSize(24);
	back.setPosition(35 - back.getGlobalBounds().width / 2, 475 - back.getGlobalBounds().height);

	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	shape.setSize(sf::Vector2f(300, 50));
	shape.setOrigin(150, 75);
	shape.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 2);

	sf::Texture t_next;
	t_next.loadFromFile("images/next.png");
	sf::Sprite s_next;
	s_next.setTexture(t_next);
	s_next.setOrigin(15, 15);
	s_next.setPosition(m_window.getSize().x / 2 + 175, m_window.getSize().y / 2 - 50);

	std::string str;
	sf::Text name("", m_pImpl->m_font, 28);
	name.setFillColor(sf::Color::Black);
	name.setPosition(centerPos.x - 145, centerPos.y + 30);

	bool isPassEnter = true;

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
			if (m_event.type == sf::Event::MouseButtonReleased && m_event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(s_next.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					if (str.size() > 0)
						return str;
				}
				if (sf::IntRect(back.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_window.clear(sf::Color::White);
					return "\0";
				}
				if (sf::IntRect(shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					isPassEnter = true;
				else
					isPassEnter = false;
			}
			if (m_event.type == sf::Event::TextEntered && isPassEnter)
			{
				if (m_event.text.unicode == '\b')
				{
					if (str.size() > 0)
					{
						str.pop_back();
						name.setString(str);
					}
				}
				else if (m_event.text.unicode < 128)
				{
					if (str.size() < 20)
					{
						str += static_cast<char>(m_event.text.unicode);
						name.setString(str);
					}
				}
			}
		}

		s_next.setScale(1, 1);
		back.setFillColor(sf::Color::Black);
		if (sf::IntRect(s_next.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			s_next.setScale(1.10, 1.10);
		}
		if (sf::IntRect(back.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			back.setFillColor(sf::Color::Blue);
		}

		m_window.clear(sf::Color::White);
		m_window.draw(enter);
		m_window.draw(back);
		m_window.draw(shape);
		m_window.draw(name);
		m_window.draw(s_next);
		m_window.display();
	}
}

char Game::SearchGame(sf::TcpSocket& socket)
{
	std::string myName = EnterName();
	if (myName == "\0")
		return '\0';

	sf::Vector2f centerPos = sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 40);
	
	sf::Text loading("   Loading . . .", m_pImpl->m_font);
	loading.setFillColor(sf::Color::Black);
	loading.setCharacterSize(30);
	loading.setStyle(sf::Text::Style::Bold);
	loading.setPosition(centerPos.x - loading.getGlobalBounds().width / 2, centerPos.y - loading.getGlobalBounds().height);

	m_window.clear(sf::Color::White);
	m_window.draw(loading);
	m_window.display();

	if (socket.connect("localhost", 55055, sf::seconds(3)) != sf::Socket::Status::Done)   //192.168.0.105
	{
		loading.setString("       Failed connection\nCheck Ethernet connection");
		loading.setCharacterSize(24);
		loading.setPosition(centerPos.x - loading.getGlobalBounds().width / 2, centerPos.y - loading.getGlobalBounds().height);

		m_window.clear(sf::Color::White);
		m_window.draw(loading);
		m_window.display();
		sf::sleep(sf::seconds(3));

		return '\0';
	}	

	sf::Packet packet;
	std::string turn, enemyName;

	socket.setBlocking(false);

	loading.setString("  Searching . . .");
	sf::Text back("Back", m_pImpl->m_font);
	back.setFillColor(sf::Color::Black);
	back.setCharacterSize(24);
	back.setPosition(35 - back.getGlobalBounds().width / 2, 475 - back.getGlobalBounds().height);

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
			if (m_event.type == sf::Event::KeyReleased)
			{
				if (m_event.key.code == sf::Keyboard::Escape)
				{
					if (DrawMenu())
					{
						socket.disconnect();
						return '\0';
					}
				}
			}
			if (m_event.type == sf::Event::MouseButtonReleased && m_event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(back.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_window.clear(sf::Color::White);
					socket.disconnect();
					return '\1';
				}
			}
		}

		if (socket.receive(packet) == sf::Socket::Done)
		{
			packet >> turn;
			packet.clear();

			packet << myName;
			socket.send(packet);
			packet.clear();

			socket.setBlocking(true);

			if (socket.receive(packet) == sf::Socket::Done)
			{
				packet >> enemyName;
				packet.clear();
			}

			if (turn == "f")
			{
				m_pImpl->m_playerOne = new Human('w');
				m_pImpl->m_playerTwo = new Human('b');
			}
			else
			{
				m_pImpl->m_playerOne = new Human('b');
				m_pImpl->m_playerTwo = new Human('w');
			}

			m_pImpl->m_playerOne->SetName(myName);
			m_pImpl->m_playerTwo->SetName(enemyName);

			socket.setBlocking(false);

			m_pImpl->m_MP = true;

			loading.setString("Start Game");
			m_window.clear(sf::Color::White);
			m_window.draw(loading);
			m_window.display();
			sf::sleep(sf::seconds(1));

			return turn[0];
		}

		back.setFillColor(sf::Color::Black);
		if (sf::IntRect(back.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			back.setFillColor(sf::Color::Blue);
		}

		m_window.clear(sf::Color::White);
		m_window.draw(loading);
		m_window.draw(back);
		m_window.display();
	}
}

void Game::Multiplayer()
{
	sf::TcpSocket socket;

	char turn;
	while (true)
	{
		turn = SearchGame(socket);
		if (turn == '\0')
			return;
		else if (turn == '\1')
			continue;
		else
			break;
	}

	sf::Packet packet;

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_window.close();
			if (m_event.type == sf::Event::KeyReleased)
			{
				if (m_event.key.code == sf::Keyboard::Escape)
				{
					if (DrawMenu())
					{
						socket.disconnect();
						return;
					}
				}
			}
		}

		DrawGame();

		if (turn == 'f')
		{
			while (m_pImpl->m_playerOne->MakeMove())
			{
				if (!m_pImpl->m_playerTwo->EatChecker())
				{
					DrawGame();

					if (!m_pImpl->m_playerOne->CanBeatAgain())
						break;
					else
					{
						packet << m_dataPacket;
						socket.send(packet);
						packet.clear();
					}
				}
				else
				{
					DrawGame();

					packet << m_dataPacket;
					socket.send(packet);
					packet.clear();
					socket.disconnect();
					m_pImpl->m_MP = false;

					AnnounceWinner(m_pImpl->m_playerOne->GetColor(), m_pImpl->m_playerOne->GetName());
					return;
				}
			}

			packet << m_dataPacket;
			socket.send(packet);
			packet.clear();

			turn = 's';
		}
		else
		{
			if (socket.receive(packet) == sf::Socket::Done)
			{
				packet >> m_dataPacket;	
				packet.clear();

				if (static_cast<Human*>(m_pImpl->m_playerTwo)->MoveMP())
				{
					if (!m_pImpl->m_playerOne->EatChecker())
					{
						DrawGame();

						if (!m_pImpl->m_playerTwo->CanBeatAgain())
							turn = 'f';
					}
					else
					{
						DrawGame();
						socket.disconnect();
						m_pImpl->m_MP = false;

						AnnounceWinner(m_pImpl->m_playerTwo->GetColor(), m_pImpl->m_playerTwo->GetName());
						return;
					}
				}
				else
					turn = 'f';
			}
		}
	}
	socket.disconnect();
}
