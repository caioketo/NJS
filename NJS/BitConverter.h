#ifndef __BITCONVERTER_H_INCLUDED__
#define __BITCONVERTER_H_INCLUDED__

#include "PCH.h"

class BitConverter {
public:
	static unsigned short ToUInt16(unsigned char *, int);
	static unsigned int ToUint32(unsigned char *, int);
	static unsigned long ToUint64(unsigned char *, int);
	static unsigned char * FromUint16(unsigned short);
	static unsigned char * FromUint32(unsigned int);
	static unsigned char * FromUint64(unsigned long);
};

#endif