#include "stdafx.h"
#include <string>
#include <map>
#include "Component.h"
#include "Windows.h"

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
class EntityManager{
private:


public:
	
	std::map<EntityID, Posizione> posizione;
	std::map<EntityID, Attacco> attacco;
	std::map<EntityID, Salute>salute;
	std::map<EntityID, Livello> livello;
	std::map<EntityID, Visuale> visuale;
	std::map<EntityID, ATB> atb;
	std::map<EntityID, EntityRespawn> RespawnTImer;
	std::map<EntityID, Skillset> skillset;
	std::map<EntityID, EntityRange> entityrange; 
	
	
	void AddSkill(EntityID id,std::string name,int danno){
		Skill NewSkill;
		NewSkill.Name = name;
		NewSkill.Danno = danno;
		skillset[id].skillset.push_back(NewSkill);
	}
};
#endif