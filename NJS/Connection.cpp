#include "PCH.h"
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
		Conn->RecvSize = recv(Conn->ClientSocket, Conn->Buffer, 9216, NULL);
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

				if (!Conn->Parse(Conn->InMessage))
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

bool Connection::Parse(NetworkMessage message)
{
	unsigned char type = message.GetByte();
	switch (type)
	{
	case ClientTypePacket::Login:
		//LoginPacket * lPacket = LoginPacket::Parse(message);

		NetworkMessage* outMessage = new NetworkMessage();
		outMessage->AddByte(1);
		outMessage->AddByte(2);
		this->Send(*outMessage);

		break;
	}
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
    unsigned char *text = new unsigned char[len + 1];
	for (int i = 0; i < len + 1; i++)
	{
		if ((nextByte + 4 + i) < 9216)
		{
			text[i] = this->Buffer[nextByte + 4 + i];
		}
	}
    
    unsigned char *unmaskedText = new unsigned char[len];

	for (int i = 0; i < len; i++)
	{
		unmaskedText[i] = text[i] ^ mask[i % 4];
	}
	std::string json(unmaskedText, unmaskedText + len);
	std::istringstream f(json);
	std::string s;
	int length = 0;

	this->InMessage.Reset();

	while (std::getline(f, s, ',')) {
		s.erase(std::remove(s.begin(), s.end(), '{'), s.end());
		s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
		if (std::string::npos == s.find("length"))
		{
			std::string i = s.substr(0, s.find(":"));	
			std::string valor = s.substr(s.find(":") + 1);
			unsigned char temp = (unsigned char)atoi(valor.c_str());
			this->InMessage.Buffer[atoi(i.c_str())] = temp;
		}
		else
		{
			std::string valorLen = s.substr(s.find(":") + 1);
			length = atoi(valorLen.c_str());
			break;
		}
    }

	this->InMessage.Length = length;
	this->InMessage.Position = 0;

	byte t = this->InMessage.GetByte();

    return true;
}

void Connection::Send(NetworkMessage message)
{
	unsigned char* frame = NULL;

	unsigned char* frameMsg = NULL;
	
	char* temp = message.JSONBuffer();
	int nMsgLen = strlen(temp);

	int nFrameLen = nMsgLen + 2;


	if ( nMsgLen <= 125 )

	{

		frame = new unsigned char[nFrameLen];

		frame[0] = 0x81;

		frame[1] = nMsgLen;

		frameMsg = frame+2;

	} else if ( nMsgLen <= 65000 )

	{

		nFrameLen += 2;

		frame = new unsigned char[nFrameLen];

		frame[0] = 0x81;

		frame[1] = 126;

		frame[2] = nMsgLen >> 8;

		frame[3] = nMsgLen & 0xff;

		frameMsg = frame + 4;

	} else

	{

		nFrameLen += 8;

		frame = new unsigned char[nFrameLen];

		frame[0] = 0x81;

		frame[1] = 127;


		frame[2] = 0;

		frame[3] = 0;

		frame[4] = 0;

		frame[5] = 0;

		frame[6] = (nMsgLen >> 24) & 0xff;

		frame[7] = (nMsgLen >> 16) & 0xff;

		frame[8] = (nMsgLen >> 8) & 0xff;

		frame[9] = (nMsgLen >> 0) & 0xff;


		frameMsg = frame + 10;

	} 

	memcpy(frameMsg, temp, nMsgLen);

	int iResult = send(this->ClientSocket, (char *)frame, nFrameLen, 0);

	if (iResult == SOCKET_ERROR)
	{
		//Error
		return;
	}
}

Connection::Connection(SOCKET sock)
{
	this->ClientSocket = sock;
	this->Handshaked = false;
	this->Connected = true;
}