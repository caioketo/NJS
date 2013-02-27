#include "Connection.h"

void Connection::Start() 
{
	WSAEVENT Tem = WSA_INVALID_EVENT;
    Tem = WSACreateEvent();
    WSAEventSelect(this->ClientSocket, Tem, FD_READ);
    while(this->Connected)
    {
        WaitForSingleObject(Tem, INFINITE);
		this->RecvSize = recv(this->ClientSocket, this->Buffer, 1024, NULL);
        if(this->RecvSize > 0)
        {
            this->Buffer[this->RecvSize - 1] = '\0';
			if (!this->Handshaked)
			{
				this->Handshaked = this->DoHandshake();
				if (!this->Handshaked)
				{
					return;
				}
			}
			else
			{
				if (!this->Parse())
				{
					return;
				}
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

std::string Connection::ComputeHandshakeSec(std::string key)
{

	std::string magicKey = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	key.append(magicKey);   

	unsigned char* sha_str = SHA1(reinterpret_cast<const unsigned char*>(key.c_str()), key.length(), nullptr);
	std::string final = base64_encode(reinterpret_cast<const unsigned char*>(sha_str), 20);

	return final;
}

bool Connection::DoHandshake()
{
	std::string teste = ComputeHandshakeSec("6p4JLlCIqXcBJol7kec4Cw==");
	std::string clientHandshake( reinterpret_cast< char const* >(this->Buffer) ) ;
    std::istringstream f(clientHandshake);
	std::string acceptKey = "";
    std::string s;    
    while (std::getline(f, s, '\n')) {
		std::cout << s << std::endl;
		if (std::string::npos != s.find("Sec-WebSocket-Key:"))
		{
			acceptKey = s.substr(s.find("Sec-WebSocket-Key:") + 19);
			acceptKey = acceptKey.substr(0, acceptKey.length() - 1);
			std::cout << acceptKey << std::endl;
		}
    }

	if (acceptKey != "")
	{
		std::string final = ComputeHandshakeSec(acceptKey);
		std::ostringstream oss;
		oss << "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
		oss << "Upgrade: WebSocket\r\n";
		oss << "Connection: Upgrade\r\n";
		oss << "Sec-WebSocket-Accept: " << final << "\r\n";
		oss << "\r\n";

		std::cout << "*****SEND*****\n";
		std::cout << oss.str() << std::endl;
		std::string teste = oss.str();
		const char * buff = teste.c_str();
		int iResult = send(this->ClientSocket, buff, teste.length(), 0);

		if (iResult == SOCKET_ERROR)
		{
			return false;
		}

		return true;
	}


	return false;
}

Connection::Connection(SOCKET sock)
{
	this->ClientSocket = sock;
	this->Handshaked = false;
	this->Connected = true;
}