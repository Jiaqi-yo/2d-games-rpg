#include "stdafx.h"
#include <iostream>
#include "conio.h"
#include "StatsGUI.h"
#include "EntityManager.h"
#include "Sistemi.h"

StatsGUI::StatsGUI(){
isOpen = false;
attack_point = 0;
health_point = 0;
}
void StatsGUI::Input(EntityManager& Entity,EntityID PlayerID){
	if(_kbhit()){
	char Tasto = _getch();
	if(Tasto == 27){
	isOpen = false;
	system("cls");
	return;
	}
	if(Entity.livello[PlayerID].PointStats > 0){
	switch(Tasto){
	case '1':
		Entity.attacco[PlayerID].Damage++;
		Entity.livello[PlayerID].PointStats--;
		attack_point++;
		break;
	case '2':
		Entity.salute[PlayerID].MaxHP++;
		Entity.livello[PlayerID].PointStats--;
		health_point++;
		break;
	default:
		break;
			}
		}
	}
}

void StatsGUI::StatsGraphic(EntityManager& Entity,EntityID PlayerID){
	MoveCursor(20,0);
	std::cout << "StatsPoints";
	MoveCursor(0,1);
	std::cout << "hai a disposizione: " << Entity.livello[PlayerID].PointStats << " Punti";
	MoveCursor(0,5);
	std::cout << "[1]Attacco: " << attack_point;
	MoveCursor(0,6);
	std::cout << "[2]Salute: " << health_point;
}
bool StatsGUI::GetisOpen(){
	return isOpen;
}
void StatsGUI::SetisOpen(bool a){
	isOpen = a;
}