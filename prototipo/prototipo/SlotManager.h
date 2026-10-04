#include "stdafx.h"
#include "EntityManager.h"
#ifndef SLOTMANAGER_H
#define SLOTMANAGER_H

class SlotManager{

private:
	bool isActive;
	int getScelta;
	int Scelta;
	std::string nameSlot;

public:
	SlotManager();
	void SlotInput(EntityManager& Entity,EntityID PlayerID);
	void SlotMenu();
	void SaveData(EntityManager& Entity,EntityID PlayerID);
	int GetChoice();
	void SetActive(bool s);
	bool GetActive();
	
};

#endif
//ricordare di creare una funzione per eliminare uno slot a scelta
//e poter cambiare il nome dello slot che pero quando si cancella avra il nome di default
