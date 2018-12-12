#ifndef __NETWORKMANAGER_H_
#define __NETWORKMANAGER_H_

#include <iostream>
#include <queue>

#include "RakNet/BitStream.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/GetTime.h"

#ifdef __cplusplus
extern "C" 
{
#endif
#pragma region Message Identifiers
	enum NetworkMessage
	{
		ID_DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
		ID_UPDATE_GAMESTATE,	// Send all player positions to client
		ID_PLAYER_JOINED,		// Send spawned player to other clients
		ID_CLIENT_POSITION_RECEIVED,	// Client -> Server
		ID_CLIENT_MESSAGE,		// Message sent from client
		ID_REQUEST_INITIAL_DATA,		// Server -> Client Server asking for initial data
		ID_INITIAL_CLIENT_DATA,			// Client -> Server	Client sending initial player data
		ID_STARTGAME,
		ID_CHAT_MESSAGE,		// Server -> All Clients, chat message
	};

	enum DataType
	{
		Default = 0,
		Nil,
		ChatMessageEvent,
		ClientDataRequestEvent,
		PlayerDataEvent,
		StartGameEvent,
	};
#pragma endregion
	
#pragma region Packet Data Structures
#pragma pack(push, 1)
	struct DefaultMessage
	{
		int typeID;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct ChatMessage
	{
		int typeID;
		char message[512];
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct GameStateUpdateMessage
	{
		int typeID;
		unsigned char useTimeStamp = ID_TIMESTAMP;
		RakNet::Time timeStamp;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct InitialDataMessage
	{
		int typeID;
		int guid;
		float x, y, z;
		float rotation;
		int isAlive;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct InitialDataRequestMessage
	{
		int typeID;
		int guid;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct PlayerDataMessage
	{
		int typeID;
		unsigned char useTimeStamp = ID_TIMESTAMP;
		RakNet::Time timeStamp;
		int guid;
		float x, y, z;
		float rotation;
		int isAlive;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct StartGameMessage
	{
		int typeID;
		int start;
	};
#pragma pack(pop)
#pragma endregion

#pragma region GameObject Data Structures
#pragma pack(push, 1)
	struct PlayerDataStruct
	{
		int guid = -1;
		float x = 0;
		float y = 0;
		float z = 0;
		float rotation = -1;
		int isAlive = 0;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct ChatDataStruct
	{
		char msg[512];
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct InitialDataRequestStruct
	{
		int guid;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct StartGameStruct
	{
		int start = 0;
	};
#pragma endregion

#pragma region Variables
	struct PlayerDataStruct players[4];
	int maxClients = 4;
	std::queue<DataType> events;
	std::queue<RakNet::Packet> packets;
#pragma endregion

#pragma region Functions
	__declspec(dllexport) void init();
	__declspec(dllexport) void connectToServer(char* ip);
	__declspec(dllexport) int receive();

	/* Data Handle Functions*/
	__declspec(dllexport) ChatDataStruct handleChatMessage(int exec);
	__declspec(dllexport) InitialDataRequestStruct handleInitialData(int exec);
	__declspec(dllexport) StartGameStruct handle_startgame(int exec);

	/* Data Send Functions*/
	__declspec(dllexport) void sendInitialPlayerData(int guid, float x, float y, float z, float rotation, int isAlive);
	__declspec(dllexport) void sendMessage(char* message);
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif	// !__NETWORKMANAGER_H