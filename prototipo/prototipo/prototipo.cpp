// prototipo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sistemi.h"
#include "Map.h"
#include "StartMenu.h"
#include "EntityManager.h"
#include "Component.h"
#include <iostream>
#include <string>
#include <conio.h>


int _tmain(int argc, _TCHAR* argv[])
 {
	 EntityManager Entity;
	 EntityID Player = Entity.CreaID();
	 Posizione p = {2, 2};
	 Entity.Posizione[Player] = p; 
	 Visuale v = {'@'};
	 Entity.Visuale[Player] = v;


	 Map MioMappa;
	 StartMenu Menu;
	 char Tasto = ' ';

		 while(Menu.GetAttivo()){
			 Menu.MenuGraphic();
			 Menu.Input();
		 }
		 if(Menu.GetChoice() == 1){
		 GameInExecution(MioMappa,Entity,Player);
	}
	return 0;
}

