#ifndef __PCH_INCLUDED__
#define __PCH_INCLUDED__

#pragma message("Compiling precompiled headers.\n")


#define BOOST_UTF8_BEGIN_NAMESPACE
#define BOOST_UTF8_END_NAMESPACE
#define BOOST_UTF8_DECL

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include "cryptopp\dll.h"
#include "cryptopp\sha.h"
#include "cryptopp\filters.h"
#include "cryptopp\hex.h"
#include <utf8.h>
#pragma comment (lib, "Ws2_32.lib")
using namespace std;

#endif