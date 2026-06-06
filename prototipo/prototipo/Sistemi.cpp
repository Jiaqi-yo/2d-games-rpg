#include "stdafx.h"
#include "iostream"
#include <conio.h>
#include "Sistemi.h"
#include "StartMenu.h"

void MuoviEntita(Map& m,EntityManager& Entity,EntityID& ID ,char Tasto){
	int y = Entity.Posizione[ID].Y;
	int x = Entity.Posizione[ID].X;
	
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
	Entity.Posizione[ID].Y = y;
	Entity.Posizione[ID].X = x;
}
void MapRedering(Map& m,EntityManager& Entity,EntityID& ID){
	int x = Entity.Posizione[ID].X;
	int y = Entity.Posizione[ID].Y;
	system("cls");
	for(int i = 0;i < 20; i++){
		for(int j = 0;j < 20; j++){
			if(i == y && j == x){
				if(i == y && j == x){
					std::cout << Entity.Visuale[ID].Simbolo;
				}
			}else if(m.GetCella(i,j) == 1){
				std::cout << '#';
			}else if(m.GetCella(i,j) == 2){
				std::cout << '&';
			}else if(m.GetCella(i,j) == 0){
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void GameInExecution(Map& m,EntityManager& Entity,EntityID& ID){
	bool GameInExecution = true;
	char Tasto = ' ';
	StartMenu Menu;

	while(GameInExecution){
	MapRedering(m, Entity, ID);
	Tasto = _getch();
	if(Tasto == 27){
		Menu.SetAttivo(true);
		while(Menu.GetAttivo()){
		Menu.MenuGraphic();
		Menu.Input();
			}
	}else{
	MuoviEntita(m, Entity, ID, Tasto);
	}
  }
}
