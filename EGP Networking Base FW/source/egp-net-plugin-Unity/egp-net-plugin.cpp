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

extern "C"
{
	// dummy function for testing
	__declspec(dllexport)	// tmp linker flag, forces lib to exist
		int foo(int bar)
	{
		return (bar + 1);
	}
}