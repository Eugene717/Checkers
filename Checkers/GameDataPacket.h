#pragma once
#include <SFML/Network.hpp>
#include <vector>

using Pos = std::pair<int, int>;

struct GameDataPacket
{
	Pos m_starterPos;
	Pos m_finishPos;
	bool m_finishGame = false;
};

sf::Packet& operator <<(sf::Packet& packet, const GameDataPacket& m);
sf::Packet& operator >>(sf::Packet& packet, GameDataPacket& m);
