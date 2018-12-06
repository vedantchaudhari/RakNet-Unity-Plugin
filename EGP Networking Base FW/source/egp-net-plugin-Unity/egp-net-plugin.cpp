/*
	EGP Networking: Plugin Interface/Wrapper
	Dan Buckstein
	October 2018

	Main implementation of Unity plugin wrapper.
*/

#include "egp-net-plugin.h"
#include "NetworkManager.h"

/* Raknet Headers */
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"

#pragma pack(push, 1)
struct GameMessageData
{
	unsigned char typeID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IntMessageData
{
	unsigned char typeID;
	int data;
};

RakNet::RakPeerInterface* peer;
NetworkManager* networkManager;

extern "C"
{
	// dummy function for testing
	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	int foo(int bar)
	{
		return (bar + 1);
	}

	__declspec(dllexport)
	char* testReturn(int* length)
	{
		char* string = "success_return_string";
		*length = sizeof(string);
		return string;
	}

	/* Networking Functions */

	__declspec(dllexport)
	int StartNetworking(char* address)
	{
		networkManager = NetworkManager::getInstance();
		networkManager->StartupNetworking(true, 0, 0, false);
		networkManager->setIP(address);
		networkManager->Connect(address, networkManager->GetPort());
		peer = networkManager->getPeer();
		return 0;
	}

	__declspec(dllexport)
	int Disconnect()
	{
		if (networkManager->Disconnect() != 0)
			return 0;
		else
			return 1;
	}

	__declspec(dllexport)
	int ShutdownNetworking()
	{
		if (networkManager->ShutdownNetworking() != 0)
			return 0;
		else
			return 1;
	}

	/* Functions for handling data received over the network */
	char* HandlePacket(int* length)
	{
		RakNet::Packet* packet;
		packet = peer->Receive();

		if (!packet)
		{
			*length = sizeof("null_packet");
			return "null_packet";
		}

		switch (packet->data[0])
		{
		case ID_CONNECTION_LOST:
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			break;
		case ID_REMOTE_CONNECTION_LOST:
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			break;
		case NetworkManager::SEND_NETWORK_INT:
			{
				RakNet::Time time;
				RakNet::RakString data;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(time);
				bsIn.Read(data);
				char* returnValue = (char*)data.C_String();
				*length = sizeof(data);
				return returnValue;
			}
			break;
		default:
			break;
		}

		return "no_data_received";
	}

	/* Functions for sending data over the network */
	__declspec(dllexport)
	int SendInteger(int data)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)NetworkManager::SEND_NETWORK_INT);
		const RakNet::Time timeStamp = RakNet::GetTime();
		bsOut.Write(timeStamp);
		bsOut.Write(data);
		networkManager->sendBitStream(&bsOut);
		return 0;
	}
}