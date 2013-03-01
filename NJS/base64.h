#ifndef __BASE64_H_INCLUDED__
#define __BASE64_H_INCLUDED__

#include "PCH.h"

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif