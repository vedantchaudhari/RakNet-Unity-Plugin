#ifndef __NETWORKMANAGER_H_
#define __NETWORKMANAGER_H_

#include <iostream>
#include <vector>

#include "RakNet/BitStream.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"

#ifdef __cplusplus
extern "C" 
{
#endif
#pragma region Message Identifiers
	enum NetworkMessages
	{
		ID_DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
		ID_UPDATE_GAMESTATE,
		ID_PLAYER_JOINED,
		ID_CLIENT_POSITION_RECEIVED,	// Client -> Server
		ID_CLIENT_MESSAGE,			// Message sent to server
		ID_REQUEST_INITIAL_DATA,	// Server -> Client asking for initial data
	};

	enum DataType
	{
		Default = 0,
		Nil,
		PlayerData,

		// Server requests
		ClientDataRequest,
	};
#pragma endregion
	
#pragma region Packet Data Structures
#pragma pack(push, 1)
	struct DefaultMessage
	{
		char typeID;
		char msg[32];
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct GameStateUpdateMessage
	{
		unsigned char useTimeStamp = ID_TIMESTAMP;
		RakNet::Time timeStamp;

		char typeID;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct PlayerDataMessage
	{
		unsigned char useTimeStamp = ID_TIMESTAMP;
		RakNet::Time timeStamp;
		char typeID;
		int guid;
		float x, y, z;
		float rotation;
	};
#pragma pack(pop)
#pragma endregion

#pragma region GameObject Data Structures
	struct PlayerData
	{
		int guid;
		float x, y, z;
		float rotation;
		int isAlive;
	};
#pragma endregion

#pragma region Variables
	struct PlayerData players[4];
	int maxClients = 4;
#pragma endregion

#pragma region Functions
	__declspec(dllexport) void init();
	__declspec(dllexport) void connectToServer(char* ip);
	__declspec(dllexport) DataType receive();
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif	// !__NETWORKMANAGER_H