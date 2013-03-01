#include "PCH.h"
#include "NetworkMessage.h"

unsigned char NetworkMessage::GetByte()
{
	return this->Buffer[this->Position++];
}

unsigned char* NetworkMessage::GetBytes(int count)
{
	unsigned char *result = new unsigned char [count];
	for (int i = 0; i < count; i++) 
	{
		result[i] = this->GetByte();
	}
	return result;
}

unsigned short NetworkMessage::GetUint16()
{
	return BitConverter::ToUInt16(this->GetBytes(2), 0);
}

string NetworkMessage::GetString()
{
	int len = this->GetUint16();
	unsigned char* temp = this->GetBytes(len);
	return std::string(temp, temp + len);
}

void NetworkMessage::Reset()
{
	this->Length = 6;
	this->Position = 6;
}

int NetworkMessage::JSONLength()
{
	if (changed)
	{
		//Calculate
	}
	return this->JSONlength;
}

unsigned char * NetworkMessage::JSONBuffer()
{
	if (changed)
	{
		//Calculate
	}
	return this->JSONbuffer;
}


NetworkMessage::NetworkMessage()
{
	this->changed = true;
	//this->Buffer = (unsigned char)*new unsigned char[1024];
}