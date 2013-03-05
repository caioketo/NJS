#include "PCH.h"
#include "Terrain.h"


Terrain::Terrain()
{
	int y = 0;
	int z = 0;
	for (int x = 0; x < 10; x++)
	{
		this->vertices[x].x = x;
		this->vertices[x].y = y;
		this->vertices[x].z = z;
		y++;
		z++;
	}
	instance = *this;
}

Terrain Terrain::GetInstance()
{
	return instance;
}