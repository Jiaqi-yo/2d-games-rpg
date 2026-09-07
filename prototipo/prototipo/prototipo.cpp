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
	 std::vector<EntityID> EnemyID;
	 EnemyID.push_back(2);
	 EnemyID.push_back(3);

	 EntityID PlayerID = 1;
	 Entity.posizione[1].X = 2;
	 Entity.posizione[1].Y = 2;
	 Entity.visuale[1].Simbolo = '@';
	 Entity.attacco[1].Damage = 0;
	 Entity.salute[1].HP = 10;
	 Entity.salute[1].MaxHP = 10;
	 Entity.atb[1].Time = 2;
	 Entity.atb[1].BarTime = 100;
	 Entity.livello[1].level = 0;
	 Entity.livello[1].MaxExp = 10;
	 Entity.livello[1].PointStats = 0;



	 EntityID SlimeID = 2;
	 Entity.posizione[2].X = 18;
	 Entity.posizione[2].Y = 18;
	 Entity.visuale[2].Simbolo = '&';
	 Entity.attacco[2].Damage = 2;
	 Entity.salute[2].HP = 5;
	 Entity.salute[2].MaxHP = 5;
	 Entity.atb[2].Time = 3;
	 Entity.atb[2].BarTime = 100;
	 Entity.RespawnTImer[2].ReX = 18;
	 Entity.RespawnTImer[2].ReY = 18;
	 Entity.RespawnTImer[2].timer = 10;
	 Entity.livello[2].Exp = 3;
	 Entity.entityrange[2].AttackRangeX = 4;
	 Entity.entityrange[2].AttackRangeY = 4;

	 EntityID OrcoID = 3;
	 Entity.posizione[3].X = 5;
	 Entity.posizione[3].Y = 5;
	 Entity.visuale[3].Simbolo = 'e';
	 Entity.attacco[3].Damage = 6;
	 Entity.salute[3].HP = 20;
	 Entity.salute[3].MaxHP = 20;
	 Entity.atb[3].Time = 2;
	 Entity.atb[3].BarTime = 100;
	 Entity.RespawnTImer[3].ReX = 5;
	 Entity.RespawnTImer[3].ReY = 5;
	 Entity.RespawnTImer[3].timer = 10;
	 Entity.livello[3].Exp = 20;


	 Entity.AddSkill(PlayerID,"Pugno",2);
	 Entity.AddSkill(PlayerID, "Pugno pesante",3);

	 Map MioMappa;
	 StartMenu Menu;

		 while(Menu.GetAttivo()){
			 Menu.MenuGraphic();
			 Menu.Input();	 	 
			 if(Menu.GetChoice() == 2 && Menu.GetAttivo() == false){
				system("cls");
			 GameInExecution(MioMappa,Entity,PlayerID,EnemyID);
	}
}
	return 0;
}



