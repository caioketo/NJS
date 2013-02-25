#include "Server.h"


DWORD WINAPI MainThread(LPVOID lParam)
{
    ((Server*)lParam)->WaitForConnections((Server*)lParam);
    return 0;
}


Server::Server()
{
	int iResult;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	this->ListenSocket = INVALID_SOCKET;

	//Constructor
	this->connections = new list<Connection *>();

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		return;
	}

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
	    WSACleanup();
		return;
	}

	this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	

	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return ;
	}

	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

	freeaddrinfo(result);

	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

Server::~Server()
{
	//Destructor
}

int Server::Start()
{
	this->Enabled = true;
	CreateThread(NULL, NULL, &MainThread, this, NULL, NULL);
	return 0;
}

void Server::WaitForConnections(Server* Ser)
{
	WSAEVENT Handler = WSA_INVALID_EVENT;
	Handler = WSACreateEvent();
	while(Ser->Enabled)
    {
		WSAEventSelect(Ser->ListenSocket, Handler, FD_ACCEPT);
        WaitForSingleObject(Handler, INFINITE);
        SOCKET accptsock = accept(Ser->ListenSocket, NULL, NULL);
		this->connections->push_back(new Connection(accptsock));
        /*Client * NewClient = new Client(accptsock, 255, Ser->ID++);
        NewClient->Connected = true;
        printf("[AsynServer]Client connected.\n");
        ClientServer * OurStruct = new ClientServer();
        OurStruct->Server = Ser;
        OurStruct->Client = NewClient;
        CreateThread(NULL, NULL, &DataThread, OurStruct, NULL, NULL);*/
	}
}