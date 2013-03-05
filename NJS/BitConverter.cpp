#include "PCH.h"
#include "BitConverter.h"


unsigned short BitConverter::ToUInt16(unsigned char * bytes, int start)
{
	return ((bytes[start]) | (bytes[start + 1] << 8));
}

unsigned int BitConverter::ToUint32(unsigned char * bytes, int start)
{
	return (bytes[start] |
                (bytes[start + 1] << 8) |
                (bytes[start + 2] << 16) |
                (bytes[start + 3] << 24));
}

unsigned long BitConverter::ToUint64(unsigned char * bytes, int start)
{
	return (bytes[start] |
        		(bytes[start + 1] << 8) |
        		(bytes[start + 2] << 16) |
        		(bytes[start + 3] << 24) |
        		(bytes[start + 4] << 32) |
        		(bytes[start + 5] << 40) |
        		(bytes[start + 6] << 48) |
        		(bytes[start + 7] << 56)
        		);
}

unsigned char * BitConverter::FromUint16(unsigned short value)
{
	unsigned char * result = new unsigned char[2];
	result[0] = value;
	result[1] = value >> 8;
	result[2] = '\0';
	return result;
}

unsigned char * BitConverter::FromUint32(unsigned int value)
{
	unsigned char * result = new unsigned char[4];
	result[0] = value;
	result[1] = value >> 8;
	result[2] = value >> 16;
	result[3] = value >> 24;
	result[4] = '\0';
	return result;
}

unsigned char * BitConverter::FromUint64(unsigned long value)
{
	unsigned char * result = new unsigned char[8];
	result[0] = value;
	result[1] = value >> 8;
	result[2] = value >> 16;
	result[3] = value >> 24;
	result[4] = value >> 32;
	result[5] = value >> 40;
	result[6] = value >> 48;
	result[7] = value >> 56;
	result[8] = '\0';
	return result;
}