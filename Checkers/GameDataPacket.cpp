#include "GameDataPacket.h"

using Pos = std::pair<int, int>;

sf::Packet& operator <<(sf::Packet& packet, const GameDataPacket& m)
{
    return packet << m.m_starterPos.first << m.m_starterPos.second << m.m_finishPos.first << m.m_finishPos.second << m.m_finishGame;
}

sf::Packet& operator >>(sf::Packet& packet, GameDataPacket& m)
{
    return packet >> m.m_starterPos.first >> m.m_starterPos.second >> m.m_finishPos.first >> m.m_finishPos.second >> m.m_finishGame;
}
