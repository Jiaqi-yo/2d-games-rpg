#include "stdafx.h"
#include "iostream"
#include "StartMenu.h"
#include "GameInExecution.h"
#include "Sistemi.h"
#include <conio.h>

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