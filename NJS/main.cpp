//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif

//#define DEFAULT_PORT "27015"
//#define DEFAULT_BUFLEN 512

//#pragma comment(lib, "Ws2_32.lib")
//#include "PCH.h"
#include "Server.h"

int main() {
	Server* s = new Server();
	s->Start();
	while (true)
	{
	}
	return 0;
}