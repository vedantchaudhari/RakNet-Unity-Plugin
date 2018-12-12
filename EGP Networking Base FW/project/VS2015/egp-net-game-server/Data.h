/*
	@author Vedant Chaudhari, 153022
	@project EGP-NETWORKING-FINAL: Floor is LAva
	@LastModified 12.06.2018

	We certify that this work is entirely our own. The assessor of this project
	may reproduce this project and provide copies to other academic staff, and/or
	communicate a copy of this project to a plagiarism-checking service, which may
	retain a copy of the project on its database.
*/


#ifndef __DATA_H
#define __DATA_H

#pragma region Includes
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <random>

#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"
#pragma endregion

#pragma region Constant Defines
#define MAX_CLIENTS 2
#pragma endregion

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
	ID_GAME_OVER,			// Server -> Clients who isAlive = false
	ID_GAME_WON,			// Server -> Client who isAlive = true
};

#pragma region GameObject Data Structures
#pragma pack(push, 1)
struct PlayerDataStruct
{
	int guid = -1;
	int playerNumber = -1;
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
struct GameStateUpdateStruct
{
	PlayerDataStruct pData[MAX_CLIENTS];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct InitialDataRequestStruct
{
	int guid;
	int playerNumber;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct StartGameStruct
{
	int start = 0;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct JoinGameStruct
{
	PlayerDataStruct pData;
};
#pragma pack(pop)
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
	PlayerDataStruct pData[MAX_CLIENTS];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct JoinGameMessage
{
	int typeID;
	int playerNumber;
	PlayerDataStruct pData;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct PlayerDataMessage
{
	int typeID;
	unsigned char useTimeStamp = ID_TIMESTAMP;
	RakNet::Time timeStamp;
	int guid;
	int playerNum;
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
	int playerNumber;
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

#pragma region Variables
struct PlayerDataStruct playerDataArr[MAX_CLIENTS];
bool isPlayerReady[MAX_CLIENTS];
#pragma endregion

#endif // !__DATA_H
