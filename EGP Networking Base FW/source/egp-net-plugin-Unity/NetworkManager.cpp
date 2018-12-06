#include "NetworkManager.h"
#include <iostream>

void NetworkManager::sendBitStream(RakNet::BitStream* bs, int peer) const
{
	SendPacket(bs, peer, true, true);
}