#include "Connection.h"

void Connection::Start() 
{
	WSAEVENT Tem = WSA_INVALID_EVENT;
    Tem = WSACreateEvent();
    WSAEventSelect(this->ClientSocket, Tem, FD_READ);
    while(this->Connected)
    {
        WaitForSingleObject(Tem, INFINITE);
		this->RecvSize = recv(this->ClientSocket, this->Buffer, 255, NULL);
        if(this->RecvSize > 0)
        {
            this->Buffer[this->RecvSize - 1] = '\0';
            if (!this->Parse())
			{
				return;
			}
            Sleep(50);
        }
    }
    return;
}

bool Connection::Parse()
{
	return true;
}

Connection::Connection(SOCKET sock)
{
	this->ClientSocket = sock;
}