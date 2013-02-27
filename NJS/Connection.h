#include "PCH.h"
#include "base64.h"
#pragma comment (lib, "libeay32.lib")
#include "openssl\sha.h"

class Connection {
private:
	SOCKET ClientSocket;
	bool Connected;
	bool Handshaked;
	int RecvSize;
	char Buffer[1024];
protected:
	std::string ComputeHandshakeSec(std::string);
	bool DoHandshake();
	bool Parse();
public:
	void Start();
	Connection(SOCKET);
};


