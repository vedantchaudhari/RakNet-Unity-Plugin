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

	virtual int ProcessPacket(const RakNet::Packet * const packet, const unsigned int packetIndex) const;

public:
	enum PacketDefinitions
	{
		ID_DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
		UPDATE_GAME_STATE,		// Server -> Client
		UPDATE_PLAYER_POSITION,	// Client -> Server

		// Test Messages
		SEND_NETWORK_INT,		// Client -> Client?
	};

	// Constructor
	NetworkManager();

	// Destructor
	~NetworkManager();

	/* Getters */
	static NetworkManager* getInstance();
	inline int getNumOfPlayers() { return mp_peer->NumberOfConnections(); };
	inline RakNet::RakPeerInterface* getPeer() { return mp_peer; };

	/* Setters */
	inline void setIP(std::string ipAddress) { this->ipAddress = ipAddress; };

	/* Send data over the network */
	void sendBitStream(RakNet::BitStream* bs, int peer = -1) const;
};

#endif	// !__NETWORKMANAGER_H