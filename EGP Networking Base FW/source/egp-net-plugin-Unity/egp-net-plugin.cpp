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

	/* Functions for sending data over the network */
	__declspec(dllexport)
	int SendInteger(char* guid, int guidLength, int data)
	{
		RakNet::BitStream bsOut;	
	}
}