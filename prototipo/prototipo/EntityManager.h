#include "stdafx.h"
#include <string>
#include <map>
#include "Component.h"
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
class EntityManager{
private:

	int NextID;

public:
	std::map<EntityID, Posizione> posizione;
	std::map<EntityID, Attacco> attacco;
	std::map<EntityID, Salute>salute;
	std::map<EntityID, Livello> livello;
	std::map<EntityID, Visuale> visuale;
	std::map<EntityID, ATB> atb;
	
	EntityManager(){
	NextID = 0;
	}

	EntityID CreaID(){
	return NextID++;
	}

	void EraseEntity(EntityID ID){
	posizione.erase(ID);
	attacco.erase(ID);
	salute.erase(ID);
	livello.erase(ID);
	visuale.erase(ID);
	atb.erase(ID);
	}

	EntityID CreateEntity(int x,int y, char simbolo,double att,double health,double bt, double tm){
		EntityID NuovoID = CreaID();

		Posizione pos;
		pos.X = x;
		pos.Y = y;
		posizione[NuovoID] = pos;

		Visuale vis;
		vis.Simbolo = simbolo;
		visuale[NuovoID] = vis;

		Attacco damage;
		damage.Damage = att;
		attacco[NuovoID] = damage;

		Salute sal;
		sal.HP = health;
		salute[NuovoID] = sal;

		ATB time;
		time.BarTime = bt;
		time.Time = tm;
		atb[NuovoID] = time;
		
		return NuovoID;
	}
};
#endif