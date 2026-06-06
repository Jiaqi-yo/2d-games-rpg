#include "stdafx.h"
#ifndef MAP_H
#define MAP_H
class Map{
private:
	int Griglia[20][20];
public:
	Map();
	int GetCella(int r,int c);
};

#endif
