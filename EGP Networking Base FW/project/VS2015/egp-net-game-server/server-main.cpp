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

	peer->Startup(maxClients, &sd, 1);
	std::cout << "Server is starting up..." << std::endl;
	peer->SetMaximumIncomingConnections(maxClients);
	std::cout << "Maximum incoming connections: " << maxClients << std::endl;

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{

			}
		}
	}
}