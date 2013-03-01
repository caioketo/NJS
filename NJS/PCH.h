#ifndef __PCH_INCLUDED__
#define __PCH_INCLUDED__

#pragma message("Compiling precompiled headers.\n")

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include "cryptopp\dll.h"
#include "cryptopp\sha.h"
#include "cryptopp\filters.h"
#include "cryptopp\hex.h"
#pragma comment (lib, "Ws2_32.lib")
using namespace std;

#endif