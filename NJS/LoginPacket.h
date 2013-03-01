#ifndef __LOGINPACKET_H_INCLUDED__
#define __LOGINPACKET_H_INCLUDED__

#include "PCH.h"
#include "NetworkMessage.h"

class LoginPacket 
{
public:
	string Account;
	string Password;
	static LoginPacket * Parse(NetworkMessage message);
};

#endif