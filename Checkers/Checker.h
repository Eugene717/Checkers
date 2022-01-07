#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>

class Checker :public sf::Drawable
{
	sf::Vector2i m_pos;
	sf::Vector2f m_posGraphic;
	char m_color;
	bool m_queen;
	sf::Sprite m_sprite;
	sf::Sprite m_queenSprite;

	static int NCheckers;
	static sf::Texture m_whiteTexture;
	static sf::Texture m_blackTexture;
	static sf::Texture m_queenTexture;

	static sf::SoundBuffer m_fastMoveBuf;
	static sf::Sound m_fastMove;
	static sf::SoundBuffer m_longMoveBuf;
	static sf::Sound m_longMove;
	static sf::SoundBuffer m_takeBeatCheckerBuf;
	static sf::Sound m_takeBeatChecker;
	static void LoadFiles();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void CheckQueen();
public:
	Checker(sf::Vector2i&& pos, char color) noexcept;
	~Checker();
	sf::FloatRect getGlobalBounds() const;
	void setPosition(const float& x, const float& y);
	sf::Vector2i GetPosition() const;
	std::pair<bool, bool> Move(const sf::Vector2f& posGraphic,const bool& canMoves);
	std::vector<sf::Vector2i> CanMove(bool* canBeat = nullptr) const;
	bool Alived() const;
};
