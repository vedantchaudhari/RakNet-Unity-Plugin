#include "NetworkManager.h"

RakNet::RakPeerInterface* clientPeer;
unsigned short port = 1111;

__declspec(dllexport)
void init()
{
	clientPeer = RakNet::RakPeerInterface::GetInstance();
}

__declspec(dllexport)
void connectToServer(char* ip)
{
	RakNet::SocketDescriptor sd;
	clientPeer->Startup(1, &sd, 1);
	char address[512];
	strcpy(address, ip);
	clientPeer->Connect(address, port, 0, 0);
}

__declspec(dllexport)
int receive()
{
	RakNet::Packet* packet;

	for (packet = clientPeer->Receive(); packet; clientPeer->DeallocatePacket(packet), packet = clientPeer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REQUEST_INITIAL_DATA:
			{
				events.push(ClientDataRequest);
				return ClientDataRequest;
			}
		case ID_UPDATE_GAMESTATE:
			{
				events.push(PlayerData);
				return PlayerData;
			}
		default:
			return Default;
		}
	}

	return Nil;
}