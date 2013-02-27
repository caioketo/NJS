#include "Connection.h"


DWORD WINAPI DataThread(LPVOID lParam)
{
    ((Connection*)lParam)->WaitForData((Connection*)lParam);
    return 0;
}

void Connection::WaitForData(Connection* Conn)
{
	WSAEVENT Tem = WSA_INVALID_EVENT;
    Tem = WSACreateEvent();
    WSAEventSelect(Conn->ClientSocket, Tem, FD_READ);
    while(Conn->Connected)
    {
        WaitForSingleObject(Tem, INFINITE);
		Conn->RecvSize = recv(Conn->ClientSocket, Conn->Buffer, 1024, NULL);
        if(Conn->RecvSize > 0)
        {
			Conn->Buffer[Conn->RecvSize] = '\0';

			if (!Conn->Handshaked)
			{
				Conn->Handshaked = Conn->DoHandshake();
				if (!Conn->Handshaked)
				{
					return;
				}
			}
			else
			{
				if (!Conn->EndRead(Conn->RecvSize))
				{
					return;
				}

				if (!Conn->Parse())
				{
					return;
				}
			}
            Sleep(50);
        }
    }
    return;
}

void Connection::Start() 
{
	CreateThread(NULL, NULL, &DataThread, this, NULL, NULL);
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

bool Connection::EndRead(int rSize)
{
	unsigned char firstByte = this->Buffer[0];
	unsigned char secondByte = this->Buffer[1];

	if (firstByte != 0x81)
	{
		return false;
	}

	if (secondByte < 0x80)
	{
		return false;
	}

	int len = secondByte & 0x7F;
	int nextByte = 2;
	if (len == 126)
    {
		unsigned char lenByte[2] = {this->Buffer[2], this->Buffer[3]};
        len = BitConverter::ToUInt16(lenByte, 0);
        nextByte = 4;
    }

    if (len == 127)
    {
		unsigned char lenByte[4] = {this->Buffer[2], this->Buffer[3], this->Buffer[4], this->Buffer[5]};
        len = BitConverter::ToUInt16(lenByte, 0);
        nextByte = 6;
    }


	unsigned char mask[4] = {this->Buffer[nextByte], this->Buffer[nextByte + 1], this->Buffer[nextByte + 2], this->Buffer[nextByte + 3]};
    unsigned char *text = new unsigned char[len];
	for (int i = 0; i < len + 1; i++)
	{
		text[i] = this->Buffer[nextByte + 4 + i];
	}
    
    unsigned char *unmaskedText = new unsigned char[len];

	for (int i = 0; i < len; i++)
	{
		unmaskedText[i] = text[i] ^ mask[i % 4];
	}

    return true;
}

Connection::Connection(SOCKET sock)
{
	this->ClientSocket = sock;
	this->Handshaked = false;
	this->Connected = true;
}