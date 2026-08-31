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
	 EntityID PlayerID = Entity.CreateEntity(2,2, '@', 2, 10,10, 100, 5,0,0,0,0,0,10);
	 EntityID SlimeID = Entity.CreateEntity(18,18,'&', 5, 5,5, 100, 3,18,18,0,2,0,0);
	 //appena messo xp ai mostri e adesso devo solamente implementarlo su combatsystem cosi per guadagnare
	
	 Entity.AddSkill(PlayerID,"Pugno",2);
	 Entity.AddSkill(PlayerID, "Pugno pesante",3);
	  // <----- mettilo nel combat system direttamente e non qua e poi metti exp con i mostri cosi puoi guadagnare tramite quali mostri sono
	 
	 
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



