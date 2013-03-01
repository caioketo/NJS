#ifndef __CONNECTION_H_INCLUDED__
#define __CONNECTION_H_INCLUDED__

#include "PCH.h"
#include "base64.h"
#pragma comment (lib, "libeay32.lib")
#include "openssl\sha.h"
#include "NetworkMessage.h"
#include "Enums.h"
#include "LoginPacket.h"
#include "BitConverter.h"



class Connection {
private:
	SOCKET ClientSocket;
	bool Connected;
	bool Handshaked;
	int RecvSize;
	char Buffer[9216];
protected:
	std::string ComputeHandshakeSec(std::string);
	bool DoHandshake();
	bool Parse(NetworkMessage);
	bool EndRead(int);
public:
	NetworkMessage InMessage;
	void Start();
	Connection(SOCKET);
	void WaitForData(Connection* Conn);
	void Send(NetworkMessage);
};

#endif