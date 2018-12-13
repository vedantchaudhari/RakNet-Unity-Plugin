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
	const float tick_rate = 25.0f;	// in ms, sends every second
	const float fu_tick_rate = tick_rate * 4.0f;	// Forced Update tick rate in ms

	unsigned int maxClients = MAX_CLIENTS;
	unsigned short port = 1111;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	RakNet::SocketDescriptor sd(port, 0);

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 1000);

	float timeCurr, timePrev, fuTimePrev;

	int numOfPlayers = 0;
	int numOfDead = 0;
	bool isGameRunning = false;

	peer->Startup(maxClients, &sd, 1);
	std::cout << "Server is starting up..." << std::endl;
	peer->SetMaximumIncomingConnections(maxClients);
	std::cout << "Maximum incoming connections: " << maxClients << std::endl;
	std::cout << "Tick rate is " << tick_rate << "ms" << std::endl;
	std::cout << "Forced Update tick rate is: " << fu_tick_rate << "ms" << std::endl;
	std::cout << "Server is online and accepting packets with ip " << peer->GetLocalIP(0) << std::endl;
	std::cout << "Chat Data Struct size:   " << sizeof(ChatDataStruct) << std::endl;
	std::cout << "Game State Update Struct size: " << sizeof(GameStateUpdateStruct) << std::endl;
	std::cout << "Game Win Struct size: " << sizeof(GameWinStruct) << std::endl;
	std::cout << "Join Game Struct size: " << sizeof(JoinGameStruct) << std::endl;
	std::cout << "Initial Data Request Struct size: " << sizeof(InitialDataRequestStruct) << std::endl;
	std::cout << "Player Data Struct size: " << sizeof(PlayerDataStruct) << std::endl;
	std::cout << "Start Game Struct size: " << sizeof(StartGameStruct) << std::endl;

	timePrev = (float)RakNet::GetTimeMS();
	fuTimePrev = (float)RakNet::GetTimeMS();

	while (1)
	{
		timeCurr = (float)RakNet::GetTimeMS();

		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_DEFAULT_MESSAGE:
				break;
			case ID_CONNECTION_LOST:
				std::cout << "WARNING: PLAYER CONNECTION LOST" << std::endl;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "WARNING: SERVER HAS DISCONNECTED" << std::endl;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A connection is incoming with system address: " << packet->systemAddress.ToString() << std::endl;
				numOfPlayers++;
				InitialDataRequestMessage msg;
				msg.typeID = ID_REQUEST_INITIAL_DATA;
				msg.guid = (int)distr(eng);
				msg.playerNumber = numOfPlayers;
				std::cout << "Generated GUID: " << msg.guid << std::endl;
				std::cout << "Player number:  " << msg.playerNumber << std::endl;
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
						std::cout << packet->systemAddress.ToString() << "	guid is " << playerDataArr[iter].guid << std::endl;
						playerDataArr[iter].playerNumber = pData->playerNum;
						std::cout << packet->systemAddress.ToString() << "	playerNumber is " << playerDataArr[iter].playerNumber << std::endl;
						playerDataArr[iter].x = pData->x;
						playerDataArr[iter].y = pData->y;
						playerDataArr[iter].z = pData->z;
						std::cout << "		Position is: " << playerDataArr[iter].x << ", " << playerDataArr[iter].y << ", "
							<< playerDataArr[iter].z << std::endl;
						playerDataArr[iter].rotation = pData->rotation;
						std::cout << "		Rotation is " << playerDataArr[iter].rotation << std::endl;
						playerDataArr[iter].isAlive = pData->isAlive;
						std::cout << "		Alive status: " << playerDataArr[iter].isAlive << std::endl;
						isPlayerReady[iter] = true;

						/*
						 SEND PLAYER JOINED MSG
						JoinGameMessage joinMsg;
						joinMsg.typeID = ID_PLAYER_JOINED;
						joinMsg.pData = playerDataArr[iter];
						peer->Send((char*)&joinMsg, sizeof(JoinGameMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
						std::cout << "Player Join Event sent for guid " << joinMsg.pData.guid << " and player number: "
							<< joinMsg.pData.playerNumber << std::endl;
						*/
						break;
					}
					else
					{
						std::cout << "playerDataArr[" << iter << "] already exists with guid " << playerDataArr[iter].guid << std::endl;
					}
				}

				// ****TODO: Possibly move START_GAME send here
				// Send start game message if max amount of clients have connected
				if (numOfPlayers == 2 && !isGameRunning)
				{
					// TODO: Potentially send an initial playerloadstate to all clients
					std::cout << "Start Game sent to all clients" << std::endl;
					StartGameMessage startMsg;
					startMsg.typeID = ID_STARTGAME;
					startMsg.start = 1;
					peer->Send((char*)&startMsg, sizeof(StartGameMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					isGameRunning = true;
				}

				break;
			}
			case ID_CLIENT_MESSAGE:
			{
				std::cout << "Received chat message from " << packet->systemAddress.ToString() << std::endl;
				ChatMessage* chatMsg = (ChatMessage*)packet->data;
				ChatMessage sendMsg;
				sendMsg.typeID = ID_CHAT_MESSAGE;
				sendMsg.playerNumber = chatMsg->playerNumber;
				strcpy(sendMsg.message, chatMsg->message);
				std::cout << "Player Number: " << chatMsg->playerNumber << " Chat Message: " << chatMsg->message << std::endl;
				peer->Send((char*)&sendMsg, sizeof(ChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				break;
			}
			case ID_CLIENT_POSITION_RECEIVED:
			{
				PlayerDataMessage* pDataMsg = (PlayerDataMessage*)packet->data;
				int playerNum = pDataMsg->playerNum - 1;
				playerDataArr[playerNum].x = pDataMsg->x;
				playerDataArr[playerNum].y = pDataMsg->y;
				playerDataArr[playerNum].z = pDataMsg->z;
				if (DEBUG)
				{
					std::cout << "Client position received from player number: " << pDataMsg->playerNum << " | ";
					std::cout << playerDataArr[playerNum].x << ", " << playerDataArr[playerNum].y << ", " << playerDataArr[playerNum].z << ", "
						<< playerDataArr[playerNum].rotation << ", " << playerDataArr[playerNum].isAlive << std::endl;
				}
				playerDataArr[playerNum].rotation = pDataMsg->rotation;
				playerDataArr[playerNum].isAlive = pDataMsg->isAlive;
				if (pDataMsg->isAlive == 0)
				{
					numOfDead++;
				}
				if (DEBUG)
				{
					if (pDataMsg->isAlive == 0)
					{
						std::cout << "Client number " << pDataMsg->playerNum << " is alive: " << pDataMsg->isAlive << std::endl;
					}
				}
				break;
			}
			default:
				std::cout << "A message with identifier " << packet->data[0] << " from system address " << packet->systemAddress.ToString() << " has been received" << std::endl;
				break;
			}
		}

		/*TODO Send Game State Update*/
		// TODO ADD CHECK FOR IF IS RUNNING
		if (timeCurr - timePrev >= tick_rate && isGameRunning)
		{
			timePrev = timeCurr;
			// Send mock game state update
			GameStateUpdateMessage updateMsg;
			updateMsg.typeID = ID_UPDATE_GAMESTATE;
			updateMsg.timeStamp = RakNet::GetTime();
			updateMsg.pData[0] = playerDataArr[0];
			updateMsg.pData[1] = playerDataArr[1];
			peer->Send((char*)&updateMsg, sizeof(GameStateUpdateMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
		if (timeCurr - fuTimePrev >= fu_tick_rate && isGameRunning)
		{
			fuTimePrev = timeCurr;
			GameStateUpdateMessage updateMsg;
			updateMsg.typeID = ID_FORCE_STATE_UPDATE;
			updateMsg.timeStamp = RakNet::GetTime();
			updateMsg.pData[0] = playerDataArr[0];
			updateMsg.pData[1] = playerDataArr[1];
			peer->Send((char*)&updateMsg, sizeof(GameStateUpdateMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			if (DEBUG)
			{
				std::cout << "FORCED STATE UPDATE SENT" << std::endl;
			}
		}
		if (numOfDead == maxClients - 1)
		{
			GameWinMessage winMsg;
			winMsg.typeId = ID_GAME_OVER;
			for (int iter = 0; iter < (int)maxClients; iter++)
			{
				if (playerDataArr[iter].isAlive == 1)
				{
					winMsg.guid = playerDataArr[iter].guid;
					winMsg.winnerNum = playerDataArr[iter].playerNumber;
				}
			}
			peer->Send((char*)&winMsg, sizeof(GameWinMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
	}
}