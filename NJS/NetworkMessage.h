#ifndef __NETWORK_H_INCLUDED__
#define __NETWORK_H_INCLUDED__

#include "PCH.h"
#include "BitConverter.h"

class NetworkMessage {
private:
	int JSONlength;
	unsigned char * JSONbuffer;
	bool changed;

public:
	unsigned char Buffer[9216];
	int Length;
	int Position;

	int JSONLength();
	unsigned char * JSONBuffer();

	NetworkMessage();
	void Reset();

	unsigned char* GetBytes(int);
	unsigned char GetByte();
	string GetString();
	unsigned short GetUint16();
};

#endif