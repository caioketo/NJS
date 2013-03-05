#ifndef __TERRAIN_H_INCLUDED__
#define __TERRAIN_H_INCLUDED__

#include "PCH.h"

const int VERTICES_SIZE = 10;

class Terrain {
private:
public:
	Terrain();
	Vertex vertices[VERTICES_SIZE];
	static Terrain GetInstance();
};

static Terrain instance;

#endif