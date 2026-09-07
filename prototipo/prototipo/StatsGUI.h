#include "stdafx.h"
#include "EntityManager.h"
#ifndef StatsGUI_H
#define StatsGUI_H
class StatsGUI{
private:
	bool isOpen;
	int attack_point;
	int health_point;
public:
	StatsGUI();
	void Input(EntityManager& Entity,EntityID PlayerID);
	void StatsGraphic(EntityManager& Entity,EntityID PlayerID);
	bool GetisOpen();
	void SetisOpen(bool a);
};
#endif