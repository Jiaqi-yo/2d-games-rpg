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
	std::map<EntityID, Posizione> Posizione;
	std::map<EntityID, Attacco> Attacco;
	std::map<EntityID, Salute> Salute;
	std::map<EntityID, Livello> Livello;
	std::map<EntityID, Visuale> Visuale;
	
	EntityManager(){
	NextID = 0;
	}

	EntityID CreaID(){
	return NextID++;
	}

	void EraseEntity(EntityID ID){
	Posizione.erase(ID);
	Attacco.erase(ID);
	Salute.erase(ID);
	Livello.erase(ID);
	Visuale.erase(ID);
	}
};
#endif