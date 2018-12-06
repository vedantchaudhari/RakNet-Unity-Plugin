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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <vector>

#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"

enum NetworkMessages
{
	ID_DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
	ID_UPDATE_GAMESTATE,	// Send all player positions to client
	ID_PLAYER_JOINED,		// Send spawned player to other clients

};

#pragma pack(push, 1)
struct GameStateUpdateMessage
{
	unsigned char useTimeStamp = ID_TIMESTAMP;
	RakNet::Time timeStamp;
	
	char typeID = ID_UPDATE_GAMESTATE;
};
#pragma pack(pop)

struct PlayerData
{
	int guid;
	float x, y, z;

};

#endif // !__DATA_H
