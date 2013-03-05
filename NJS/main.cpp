#include "PCH.h"
#include "Server.h"
#include "Terrain.h"

int main() {
	Terrain* t = new Terrain();
	Server* s = new Server();
	s->Start();
	while (true)
	{
	}
	return 0;
}