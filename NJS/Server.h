#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "PCH.h"
#include "Connection.h"

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512



class Server {
private:
	list<Connection*>* connections;
	WSADATA wsaData;
	
public:
	bool Enabled;
	SOCKET ListenSocket;
	Server();
	~Server();
	int Start();
	void WaitForConnections(Server*);
};

