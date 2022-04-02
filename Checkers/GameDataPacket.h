#pragma once
#include <SFML/Network.hpp>
#include <vector>

struct GameDataPacket
{
	sf::Vector2i m_starterPos;
	sf::Vector2i m_finishPos;
	bool m_finishGame = false;
};

sf::Packet& operator <<(sf::Packet& packet, const GameDataPacket& m);
sf::Packet& operator >>(sf::Packet& packet, GameDataPacket& m);
