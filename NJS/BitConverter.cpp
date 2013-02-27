#include "BitConverter.h"


int BitConverter::ToUInt16(unsigned char * bytes, int start)
{
	return ((bytes[start]) | (bytes[start + 1] << 8));
}