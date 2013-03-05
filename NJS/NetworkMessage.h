#ifndef __NETWORK_H_INCLUDED__
#define __NETWORK_H_INCLUDED__

#include "PCH.h"
#include "BitConverter.h"

class NetworkMessage {
private:
	int JSONlength;
	

public:	
	std::string JSONbuffer;
	unsigned char Buffer[9216];
	int Length;
	int Position;

	int JSONLength();
	char * JSONBuffer();

	NetworkMessage();
	void Reset();

	unsigned char* GetBytes(int);
	unsigned char GetByte();
	string GetString();
	unsigned short GetUint16();

	void AddJSON(unsigned char *);
	void AddByte(unsigned char);
	void AddBytes(unsigned char *, int);
	void AddUint16(unsigned short);
	void AddUint32(unsigned int);
	void AddUint64(unsigned long);
	void AddFloat(float);
	void AddVertex(Vertex);
};

#endif