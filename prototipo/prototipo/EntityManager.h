#include "stdafx.h"
#include <string>
#include <map>
#include "Component.h"
#include "Windows.h"

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
	std::map<EntityID, EntityRespawn> RespawnTImer;
	std::map<EntityID, Skillset> skillset;
	
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
	skillset.erase(ID);
	}

	EntityID CreateEntity(int x,int y, char simbolo,double att,double health,double maxHP,int bt, int tm,int reY,int reX,int Timer,int exp,int liv,int maxexp){
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
		sal.MaxHP = maxHP;
		salute[NuovoID] = sal;

		ATB time;
		time.BarTime = bt;
		time.Time = tm;
		atb[NuovoID] = time;

		EntityRespawn RespawnTime;
		
		RespawnTime.ReX = reX;
		RespawnTime.ReY = reY;
		RespawnTime.timer = Timer;
		RespawnTImer[NuovoID] = RespawnTime;

		Livello lvl;
		lvl.Exp = exp;
		lvl.level = liv;
		lvl.MaxExp = maxexp;
		livello[NuovoID] = lvl;
		
		return NuovoID;
	}
	void AddSkill(EntityID id,std::string name,int danno){
		Skill NewSkill;
		NewSkill.Name = name;
		NewSkill.Danno = danno;
		skillset[id].skillset.push_back(NewSkill);
	}
};
#endif