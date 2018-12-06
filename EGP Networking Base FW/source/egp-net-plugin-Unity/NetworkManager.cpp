#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager() {}
NetworkManager::~NetworkManager() {}

int NetworkManager::ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const
{
	return 0;
}

void NetworkManager::sendBitStream(RakNet::BitStream* bs, int peer) const
{
	SendPacket(bs, peer, true, true);
}

NetworkManager* NetworkManager::getInstance()
{
	static NetworkManager instance;
	return &instance;
}