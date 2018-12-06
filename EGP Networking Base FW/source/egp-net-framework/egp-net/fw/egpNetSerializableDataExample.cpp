/*
	egpNetSerializableDataExample.cpp
	By Dan Buckstein
	(c) 2017-2018

	Source for simple serializable data component.

	Additional contributions by (and date): 

*/

#include "egp-net/fw/egpNetSerializableDataExample.h"


egpSerializableDataExample::egpSerializableDataExample(const int someInt, const float someFlt, const double someDbl, const unsigned int dynArrayLen)
{
	// ****TO-DO: 
	// init internal data
}

egpSerializableDataExample::~egpSerializableDataExample()
{
	// ****TO-DO: 
	// dealloc pointer based things OWNED BY THIS
	// DO NOT DELETE THINGS IF THEY ARE OWNED AND MANAGED ELSEWHERE!!!
}

int egpSerializableDataExample::Serialize(RakNet::BitStream *bs) const
{
	// ****TO-DO: 
	// stuff all data owned by this into array
	// do so in a way that it could be read back again
	if (bs)
	{

	}
	return 0;
}

int egpSerializableDataExample::Deserialize(RakNet::BitStream *bs)
{
	// ****TO-DO: 
	// fundamentally the opposite of serialize
	// read things back in the same order they would be written
	// do allocations and deallocations as needed
	if (bs)
	{

	}
	return 0;
}
