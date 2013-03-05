#include "PCH.h"
#include "NetworkMessage.h"


inline void encode_utf8(const std::wstring& wstr, std::string& bytes)
{
    utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(bytes));
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

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
	return strlen((const char*)this->JSONBuffer());
}

char * NetworkMessage::JSONBuffer()
{
	std::stringstream ss;
	ss << "{" << this->JSONbuffer << "\"length\":" << this->Length << ",\"byteLength\":" << this->Length << ",\"byteOffset\":0,\"buffer\":{\"byteLength\":" << this->Length << "}}";

	std::string s = ss.str();

	char * writable = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), writable);
	writable[s.size()] = '\0'; 
	return writable;
}


NetworkMessage::NetworkMessage()
{
	this->Reset();
}

void NetworkMessage::AddByte(unsigned char value)
{
	AddJSON(new unsigned char(value));
	this->Position++;
}

void NetworkMessage::AddJSON(unsigned char * value)
{	
	std::stringstream ss;
	std::string temp;
	ss << '"' << this->Position << '"' << ":";
	temp = ss.str();
	bool found = false;
	int pos = -1;
	if (this->JSONbuffer == "")
	{
		pos = 0;
	}
	else
	{
		if (std::string::npos != this->JSONbuffer.find(temp))
		{
			//achou
			int i = 1;
			std::string temp2;
			while (!found)
			{
				i++;
				ss.str(std::string());
				ss << '"' << (this->Position + i)<< '"' << ":";
				temp2 = ss.str();
				found = (std::string::npos != this->JSONbuffer.find(temp2));
			}
			int nextPos = this->JSONbuffer.find(temp2);
			this->JSONbuffer.erase(this->JSONbuffer.find(temp),this->JSONbuffer.find(temp) + strlen((this->JSONbuffer.substr(this->JSONbuffer.find(temp), nextPos - this->JSONbuffer.find(temp)).c_str())));
			pos = this->JSONbuffer.find(temp);
		}
		else
		{
			//nao achou
			int i = 1;
			ss.str(std::string());
			ss << '"' << (this->Position + i)<< '"' << ":";
			temp = ss.str();
			found = (std::string::npos != this->JSONbuffer.find(temp));

			if (!found)
			{
				pos = this->JSONbuffer.find(",") + 1;
			}
		}
	}
	ss.str(std::string());
	int it = (unsigned char)*value;
	ss << '"' << (this->Position)<< '"' << ":" << it << ',';
	this->JSONbuffer.insert(pos, ss.str());
}