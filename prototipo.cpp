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
	 EntityID PlayerID = Entity.CreateEntity(2,2, '@', 2, 10, 100, 2);
	 EntityID SlimeID = Entity.CreateEntity(18,18,'&', 0, 5, 100, 5);
	 Map MioMappa;
	 StartMenu Menu;

		 while(Menu.GetAttivo()){
			 Menu.MenuGraphic();
			 Menu.Input();	 	 
			 if(Menu.GetChoice() == 2 && Menu.GetAttivo() == false){
				 system("cls");
		 GameInExecution(MioMappa,Entity,PlayerID);
	}
}
	return 0;
}



