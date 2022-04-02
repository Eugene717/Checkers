#include "GameDataPacket.h"

sf::Packet& operator <<(sf::Packet& packet, const GameDataPacket& m)
{
    return packet << m.m_starterPos.x << m.m_starterPos.y << m.m_finishPos.x << m.m_finishPos.y << m.m_finishGame;
}

sf::Packet& operator >>(sf::Packet& packet, GameDataPacket& m)
{
    return packet >> m.m_starterPos.x >> m.m_starterPos.y >> m.m_finishPos.x >> m.m_finishPos.y >> m.m_finishGame;
}
