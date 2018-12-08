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
DataType receive()
{
	RakNet::Packet* packet;

	for (packet = clientPeer->Receive(); packet; clientPeer->DeallocatePacket(packet), packet = clientPeer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REQUEST_INITIAL_DATA:
			{
				const DefaultMessage* receivedData = (DefaultMessage*)packet->data;  
				return ClientDataRequest;
			}
			break;
		case ID_UPDATE_GAMESTATE:
			{
				const PlayerDataMessage* receivedData = (PlayerDataMessage*)packet->data;

				return PlayerData;
			}
			break;
		default:
			return Default;
			break;
		}
	}

	return Nil;
}