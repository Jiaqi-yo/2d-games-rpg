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

	double Time = Entity.atb[def].Time;
	double Bar = Entity.atb[def].BarTime;
	



}




void GameInExecution(Map& m,EntityManager& Entity,EntityID& ID){
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
		
		BarTime += 0.1;
		MoveCursor(0, 1);
		
		
		

	 if(BarTime < 100){
		 MoveCursor(0,1);
		 std::cout << "Combattimento! Caricamento:" << BarTime << "%   " << std::endl;
	 }else{
	 std::cout << "La Barra e piena " << BarTime << "%                 " << std::endl;		
		if(_kbhit()){
		char Combat = _getch();
		if(Combat == 'q'){
			MoveCursor(0,1);
			std::cout << "Hai attaccato!                            " << std::endl;
			Sleep(1000);
			BarTime = 0;
		}		
	}
}
	

	}
	Sleep(20);
	}
}


