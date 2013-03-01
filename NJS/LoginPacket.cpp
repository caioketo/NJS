#include "PCH.h"
#include "LoginPacket.h"

LoginPacket* LoginPacket::Parse(NetworkMessage message)
{
	LoginPacket* packet = new LoginPacket();
	packet->Account = message.GetString();
	packet->Password = message.GetString();
	return packet;
}