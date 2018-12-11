#include "NetworkManager.h"

#define DEBUG true

RakNet::RakPeerInterface* clientPeer;
unsigned short port = 1111;

RakNet::SystemAddress host;

__declspec(dllexport) void init()
{
	if (DEBUG)
	{
		HANDLE handle = GetConsoleWindow();
		int status = AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}

	clientPeer = RakNet::RakPeerInterface::GetInstance();
	std::cout << "init(): Initializing client peer" << std::endl;
}

__declspec(dllexport) void connectToServer(char* ip)
{
	RakNet::SocketDescriptor sd;
	clientPeer->Startup(1, &sd, 1);
	char address[512];
	strcpy(address, ip);
	std::cout << "connectToServer(): Connecting to server at address: " << address << std::endl;
	clientPeer->Connect(address, port, 0, 0);
}

__declspec(dllexport) int receive()
{
	RakNet::Packet* packet;

	for (packet = clientPeer->Receive(); packet; clientPeer->DeallocatePacket(packet), packet = clientPeer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "receive(): Our connection request has been accepted" << std::endl;
			host = packet->systemAddress;
			break;
		case ID_REQUEST_INITIAL_DATA:
			{
				std::cout << "receive(): Received ClientDataRequest from server" << std::endl;
				events.push(ClientDataRequest);
				return ClientDataRequest;
			}
		case ID_UPDATE_GAMESTATE:
			{
				std::cout << "receive(): Player data from server" << std::endl;
				events.push(PlayerData);
				return PlayerData;
			}
		default:
			std::cout << "receive(): Received default packet from server" << std::endl;
			return Default;
		}
	}

	return Nil;
}

__declspec(dllexport) void sendInitialPlayerData(int guid, float x, float y, float z, float rotation, int isAlive)
{
	PlayerDataMessage pDataMsg;
	pDataMsg.typeID = ID_INITIAL_CLIENT_DATA;
	pDataMsg.timeStamp = RakNet::GetTime();
	pDataMsg.guid = guid;
	pDataMsg.x = x;
	pDataMsg.y = y;
	pDataMsg.z = z;
	pDataMsg.rotation = rotation;
	pDataMsg.isAlive = isAlive;
	clientPeer->Send((char*)&pDataMsg, sizeof(PlayerDataMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host, false);
}