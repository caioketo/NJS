#include "PCH.h"

class Connection {
private:
	SOCKET ClientSocket;
	bool Connected;
	int RecvSize;
	char Buffer[255];
protected:
	bool Parse();
public:
	void Start();
	Connection(SOCKET);
};


