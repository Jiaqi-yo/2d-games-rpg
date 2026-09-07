#include "stdafx.h"
#include <string>
#include <vector>
#ifndef COMPONENT_H
#define COMPONENT_H

typedef int EntityID;
struct Posizione{ int X, Y;};
struct Attacco{double Damage;};
struct Skill{std::string Name;int Danno;};
struct Skillset{
std::vector<Skill> skillset;
int skillImparate;
};

struct Salute{double HP, MaxHP;};
struct Livello{
	int Exp;
	int level;
	int MaxExp;
	int PointStats;
};
struct Visuale{char Simbolo;};
struct ATB{int BarTime, Time;};


struct EntityRespawn{
	int ReX;
	int ReY;
	int timer;
};

struct EntityRange{int AttackRangeX, AttackRangeY;};



#endif

