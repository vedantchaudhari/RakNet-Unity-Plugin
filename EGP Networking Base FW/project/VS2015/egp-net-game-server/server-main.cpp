/*
@author Vedant Chaudhari, 153022
@project EGP-NETWORKING-FINAL: Floor is LAva
@LastModified 12.06.2018

We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.
*/

#include "Data.h"

int main()
{
	const float tick_rate = 60.0f;

	unsigned int maxClients = 4;
	unsigned short port = 1111;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	RakNet::SocketDescriptor sd(port, 0);

	peer->Startup(maxClients, &sd, 1);
	std::cout << "Server is starting up..." << std::endl;
	peer->SetMaximumIncomingConnections(maxClients);
	std::cout << "Maximum incoming connections: " << maxClients << std::endl;
	std::cout << "Server is online and accepting packets with ip " << peer->GetLocalIP(0) << std::endl;
	std::cout << "Player Data Struct size: " << sizeof(PlayerDataStruct) << std::endl;
	
	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_DEFAULT_MESSAGE:
				break;
			case ID_CONNECTION_LOST:
				std::cout << "WARNING: SERVER CONNECTION LOST" << std::endl;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "WARNING: SERVER HAS DISCONNECTED" << std::endl;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A connection is incoming with system address: " << packet->systemAddress.ToString() << std::endl;
				DefaultMessage msg;
				msg.typeID = ID_REQUEST_INITIAL_DATA;
				peer->Send((char*)&msg, sizeof(DefaultMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				std::cout << "Requesting initial client data from " << packet->systemAddress.ToString() << std::endl;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				std::cout << "The server is full" << std::endl;
			case ID_REMOTE_CONNECTION_LOST:
				std::cout << "Another client has lost the connection | system address: " << packet->systemAddress.ToString() << std::endl;
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected" << std::endl;
				break;
			// Custom Packet Identifiers and Handler
			case ID_INITIAL_CLIENT_DATA:
			{
				
			}
			default:
				std::cout << "A message with identifier " << packet->data[0] << " from system address " << packet->systemAddress.ToString() << " has been received" << std::endl;
				break;
			}
		}
	}
}