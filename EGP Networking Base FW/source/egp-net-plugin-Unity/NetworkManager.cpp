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

	std::cout << "|||||============ NEW PLUGIN RUN ============|||||" << std::endl;
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
			events.push(ClientDataRequestEvent);
			packets.push(*packet);

			InitialDataRequestMessage* initialMsg = (InitialDataRequestMessage*)packet;
			std::cout << "receive(): guid " << (int)initialMsg->guid << std::endl;

			return ClientDataRequestEvent;
		}
		case ID_UPDATE_GAMESTATE:
		{
			std::cout << "receive(): Player data from server" << std::endl;
			events.push(PlayerDataEvent);
			packets.push(*packet);
			return PlayerDataEvent;
		}
		case ID_CHAT_MESSAGE:
		{
			std::cout << "receive(): Chat message received from server" << std::endl;
			events.push(ChatMessageEvent);
			packets.push(*packet);
			return ChatMessageEvent;
		}
		case ID_STARTGAME:
		{
			std::cout << "receive(): START GAME RECEIVED FROM SERVER" << std::endl;
			events.push(StartGameEvent);
			packets.push(*packet);
			return StartGameEvent;
		}
		default:
			std::cout << "receive(): Received default packet from server" << std::endl;
			return Default;
		}
	}

	return Nil;
}

/* Data Handle "Fun"ctions */
__declspec(dllexport) ChatDataStruct handleChatMessage(int exec)
{
	ChatMessage* chatMsg = (ChatMessage*)packets.front().data;
	ChatDataStruct tmpChatData;
	strcpy(tmpChatData.msg, chatMsg->message);
	packets.pop();
	return tmpChatData;
}

__declspec(dllexport) InitialDataRequestStruct handleInitialData(int exec)
{
	InitialDataRequestMessage* initialMsg = (InitialDataRequestMessage*)packets.front().data;
	InitialDataRequestStruct tmpInitData;
	tmpInitData.guid = initialMsg->guid;

	std::cout << "handle_initialdata(): local guid set to " << tmpInitData.guid << std::endl;
	packets.pop();
	std::cout << "handle_initialdata(): queue::packets size: " << packets.size() << std::endl;
	return tmpInitData;
}

__declspec(dllexport) StartGameStruct handle_startgame(int exec)
{
	StartGameMessage* startMsg = (StartGameMessage*)packets.front().data;
	StartGameStruct tmpData;
	tmpData.start = startMsg->start;
	packets.pop();
	return tmpData;
}

/* Data Send Functions*/
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

__declspec(dllexport) void sendMessage(char* string)
{
	char message[512];
	strcpy(message, string);
	ChatMessage chatMsg;
	chatMsg.typeID = ID_CLIENT_MESSAGE;
	strcpy(chatMsg.message, message);
	clientPeer->Send((char*)&chatMsg, sizeof(ChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, host, false);
}