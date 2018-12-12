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

	unsigned int maxClients = 2;
	unsigned short port = 1111;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	RakNet::SocketDescriptor sd(port, 0);

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 1000);

	int numOfPlayers = 0;

	peer->Startup(maxClients, &sd, 1);
	std::cout << "Server is starting up..." << std::endl;
	peer->SetMaximumIncomingConnections(maxClients);
	std::cout << "Maximum incoming connections: " << maxClients << std::endl;
	std::cout << "Server is online and accepting packets with ip " << peer->GetLocalIP(0) << std::endl;
	std::cout << "Chat Data Struct size:   " << sizeof(ChatDataStruct) << std::endl;
	std::cout << "Initial Data Request Struct size: " << sizeof(InitialDataRequestStruct) << std::endl;
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
				InitialDataRequestMessage msg;
				msg.typeID = ID_REQUEST_INITIAL_DATA;
				msg.guid = (int)distr(eng);
				std::cout << "Generated GUID: " << msg.guid << std::endl;
				peer->Send((char*)&msg, sizeof(InitialDataRequestMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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
				std::cout << "Received initial client data from " << packet->systemAddress.ToString() << std::endl;
				PlayerDataMessage* pData = (PlayerDataMessage*)packet->data;
				for (int iter = 0; iter < (int)maxClients; iter++)
				{
					if (playerDataArr[iter].guid == -1)
					{
						playerDataArr[iter].guid = pData->guid;
						playerDataArr[iter].x = pData->x;
						playerDataArr[iter].y = pData->y;
						playerDataArr[iter].z = pData->z;
						playerDataArr[iter].rotation = pData->rotation;
						playerDataArr[iter].isAlive = pData->isAlive;
					}
					else
					{
						std::cout << "playerDataArr[" << iter << "] already exists with guid " << playerDataArr[iter].guid << std::endl;
					}
				}
				numOfPlayers++;
			}
			case ID_CLIENT_MESSAGE:
			{
				std::cout << "Received chat message from " << packet->systemAddress.ToString() << std::endl;
				ChatMessage* chatMsg = (ChatMessage*)packet->data;
				char str[512];
				strcpy(chatMsg->message, str);
				ChatMessage sendMsg;
				sendMsg.typeID = ID_CHAT_MESSAGE;
				strcpy(sendMsg.message, str);
				peer->Send((char*)&sendMsg, sizeof(ChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			default:
				std::cout << "A message with identifier " << packet->data[0] << " from system address " << packet->systemAddress.ToString() << " has been received" << std::endl;
				break;
			}
		}

		// Send start game message if max amount of clients have connected
		if (numOfPlayers == maxClients)
		{
			StartGameMessage startMsg;
			startMsg.typeID = ID_STARTGAME;
			startMsg.start = 1;
			peer->Send((char*)&startMsg, sizeof(StartGameMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
	}
}