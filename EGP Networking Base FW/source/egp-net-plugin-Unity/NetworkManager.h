#ifndef __NETWORKMANAGER_H_
#define __NETWORKMANAGER_H_

#include "egp-net/fw/egpNetPeerManager.h"
#include <vector>
#include <string>

class NetworkManager : public egpNetPeerManager
{
private:
	const unsigned int serverPort = 1111;
	const float tickRate = 60.0f;	// Ticks in ms
	
	std::string ipAddress;

public:
	enum PacketDefinitions
	{
		ID_DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
		UPDATE_GAME_STATE,		// Server -> Client
		UPDATE_PLAYER_POSITION,	// Client -> Server
	};

	// Constructor
	NetworkManager();

	// Destructor
	~NetworkManager();

	/* Getters */
	static NetworkManager* getInstance();
	inline int GetNumOfPlayers() { return mp_peer->NumberOfConnections(); };
	inline RakNet::RakPeerInterface* getPeer() { return mp_peer; };

	/* Setters */
	inline void setIP(std::string ipAddress) { this->ipAddress = ipAddress; };
};

#endif	// !__NETWORKMANAGER_H