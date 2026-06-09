#include "stdafx.h"
#include "iostream"
#include <conio.h>
#include "Sistemi.h"
#include "StartMenu.h"
#include "Windows.h"

void MuoviEntita(Map& m,EntityManager& Entity,EntityID& ID ,char Tasto){
	int y = Entity.posizione[ID].Y;
	int x = Entity.posizione[ID].X;
	
	if(Tasto == 'w'){
		if(m.GetCella(y - 1,x)!= 1){
		y--;
		}
	}
	if(Tasto == 's'){
		if(m.GetCella(y + 1,x)!= 1){
		y++;
		}
	} 
	if(Tasto == 'a'){
		if(m.GetCella(y,x - 1)!= 1){
		x--;
		}
	}
	if(Tasto == 'd'){
		if(m.GetCella(y,x + 1)!= 1){
		x++;
		}
	} 
	Entity.posizione[ID].Y = y;
	Entity.posizione[ID].X = x;
}
void MoveCursor(int x, int y){
	COORD coord;
	coord.Y = y;
	coord.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void MapRedering(Map& m,EntityManager& Entity,EntityID& ID){
	MoveCursor(0, 0);
	for(int i = 0;i < 20; i++){
		for(int j = 0;j < 20; j++){
			bool EntityFound = false;

			for(std::map<EntityID,Posizione>::iterator IT = Entity.posizione.begin(); IT != Entity.posizione.end(); ++IT){
			
			EntityID Id = IT->first;
			Posizione posizione = IT->second;
			if(IT->second.Y == i && IT->second.X == j){
			std::cout << Entity.visuale[IT->first].Simbolo;
			EntityFound = true;
			break;
			}
		}
			if(!EntityFound){
			if(m.GetCella(i,j) == 1){
				std::cout << '#';
			}else if(m.GetCella(i,j) == 0){
				std::cout << ".";
				}
			}
		}
		std::cout << std::endl;
	}
}
bool CollisionDetection(EntityManager& Entity, EntityID& ID){

	Posizione posPlayer = Entity.posizione[ID];

	for(std::map<EntityID,Posizione>::iterator IT = Entity.posizione.begin(); IT != Entity.posizione.end(); ++IT){
		Posizione PosEnemy = IT->second;
		if(IT->first != ID && IT->second.Y == posPlayer.Y && IT->second.X == posPlayer.X){
		return true;
		}
	}
	return false;
}


void CombatSystem(EntityManager& Entity, EntityID& def, EntityID& att){

	Entity.atb[def].Time++;
	Entity.atb[def].BarTime;
	Entity.atb[att].Time++;
	Entity.atb[att].BarTime;

	MoveCursor(0,1);
	std::cout << Entity.atb[def].Time;
	MoveCursor(0,2);
	std::cout << Entity.atb[att].Time;

	if(_kbhit()){
	char Combat = _getch();
	MoveCursor(5,10);
	std::cout << "SCELTA" << std::endl;
	MoveCursor(0,11);
	std::cout << "1)ATTACCO" << std::endl;
	MoveCursor(15,11);
	std::cout << "2)FUGA" << std::endl;
	if(Entity.atb[def].Time >= Entity.atb[def].BarTime && Combat == 1){
		Entity.salute[att].HP -= Entity.attacco[def].Damage;	
		MoveCursor(0,5);
		std::cout << "il giocatore ha attaccato!				" << std::endl;
		Entity.atb[def].Time -= 100;
		if(Entity.salute[att].HP <= 0){
			Entity.EraseEntity(att);
			MoveCursor(0,5);
			std::cout << "Hai Vinto!				" << std::endl;
		}
	}
	if(Entity.atb[att].Time >= Entity.atb[att].BarTime){
		Entity.salute[def].HP -= Entity.attacco[att].Damage;
		MoveCursor(0,5);
		std::cout << "il nemico ha attaccato!				" << std::endl;
		Entity.atb[att].Time -= 100;
		if(Entity.salute[def].HP <= 0){
			Entity.EraseEntity(def);
			MoveCursor(0,5);
			std::cout << "hai perso....            " << std::endl;
			}
		}else if(Combat == 2){
			MoveCursor(0,5);
			std::cout << "Sei fuggito....				" << std::endl;

		}
	}
}
void GameInExecution(Map& m,EntityManager& Entity,EntityID& ID,EntityID& att){
	bool InExecution = true;
	bool clean = false;
	double BarTime = 0;
	StartMenu Menu;
	int GameState = 0;

	while(InExecution){
		if(GameState == 0){
			if(_kbhit()){
			char Tasto = _getch();
			 if(Tasto == 27){
			Menu.SetAttivo(true);
			while(Menu.GetAttivo()){
			Menu.MenuGraphic();
			Menu.Input();
			
			}
			 }else{
				MuoviEntita(m, Entity, ID, Tasto);
				if(CollisionDetection(Entity,ID)){
				GameState = 1;
					}
				 }	
			 }
		MapRedering(m, Entity, ID);			
	}else if(GameState == 1){
		if(clean == false){
		system("cls");
		clean = true;
		}
		CombatSystem(Entity,ID,att);
	}
	Sleep(20);
	}
}


