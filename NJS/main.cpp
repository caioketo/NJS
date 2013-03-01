#include "PCH.h"
#include "Server.h"

int main() {
	Server* s = new Server();
	s->Start();
	while (true)
	{
	}
	return 0;
}