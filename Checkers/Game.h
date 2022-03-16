#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

struct GameIMPL;

class Game
{
	GameIMPL* m_pImpl;
	static Game* m_game;

	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	char FirstTurn() const;
	void SetStartedBoard();
	void AnnounceWinner(const char& color);
	std::string EnterName();
public:
	char m_board[8][8];

	sf::RenderWindow m_window;
	sf::Event m_event;
	static Game* GetInstance();
	~Game();

	bool GetSound() const;
	int MainMenu();
	//void DrawSettingsMenu();
	void DrawGame();
	void DrawPossibleMoves(const std::vector<sf::Vector2i>& pos, const bool& canMove, const char& player);
	void DrawPossibleBlows(const std::vector<sf::Vector2i>& pos);
	bool DrawMenu();
	bool Exit();

	void SinglePlayer();
	void OnePC();
	void Multiplayer();
};

#endif // !GAME_H