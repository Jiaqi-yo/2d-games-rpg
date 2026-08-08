#include "stdafx.h"
#include "iostream"
#include <conio.h>
#include "Sistemi.h"
#include "StartMenu.h"
#include "Windows.h"

const char* BLOCCO_PIENO = "\xE2\x96\x88";

void SetupUnicodeConsole() {
    SetConsoleOutputCP(CP_UTF8);
}


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

void HideCursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


void MapRedering(Map& m,EntityManager& Entity,EntityID& ID){
	for(int i = 0;i < 20; i++){
		MoveCursor(49, 0 + i);
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

int CollisionDetection(EntityManager& Entity, EntityID& ID){

	int PlayerX = Entity.posizione[ID].X;
	int PlayerY = Entity.posizione[ID].Y;
	std::map<EntityID, Posizione>::iterator IT;
	for(IT = Entity.posizione.begin(); IT != Entity.posizione.end(); ++IT){
		EntityID otherID = IT->first;
		Posizione pos = IT->second;

		if(otherID == ID)continue;

		if(pos.X == PlayerX && pos.Y == PlayerY){
		
		return otherID;
		}
	}
	return -1;
}
void CombatSystem(EntityManager& Entity, EntityID PlayerID, EntityID EnemyID, bool& clean, int& GameState) {
	SetConsoleOutputCP(CP_UTF8);
    static int BarTime = 0;
    static int EnemyBarTime = 0;

    if (!clean) {
        system("cls");
        clean = true;
        BarTime = 0;
        EnemyBarTime = 0;
    }

	if (BarTime < 100){ 
		BarTime += Entity.atb[PlayerID].Time;
		if(BarTime >= 100){
			BarTime = 100;
		}
	}
	if (EnemyBarTime < 100){
		EnemyBarTime += Entity.atb[EnemyID].Time;
		if(EnemyBarTime >= 100){
			EnemyBarTime = 100;
		}
	}

	Sleep(200);
	

  
    MoveCursor(0, 22);
    std::cout << "La tua salute:" << Entity.salute[PlayerID].HP << " HP  ";

    MoveCursor(40, 22);
    std::cout << "Salute del nemico:" << Entity.salute[EnemyID].HP << "HP  ";
	
	//separazione delle statistiche
	MoveCursor(37, 20);
	std::cout << BLOCCO_PIENO;
	MoveCursor(37, 21);
	std::cout << BLOCCO_PIENO;
	MoveCursor(37, 22);
	std::cout << BLOCCO_PIENO;
	MoveCursor(37, 23);
	std::cout << BLOCCO_PIENO;
	MoveCursor(37, 24);
	std::cout << BLOCCO_PIENO;

	 
	 MoveCursor(0, 21);
    std::cout << "Caricamento Tuo:" << BarTime << "%   ";

	 MoveCursor(40, 21);
    std::cout << "Caricamento Nemico:" << EnemyBarTime << "%     ";


    if (EnemyBarTime >= 100) {
		Sleep(100);
        Entity.salute[PlayerID].HP -= Entity.attacco[EnemyID].Damage;
        
        MoveCursor(40, 21);
        std::cout << "Il nemico ti ha attaccato!";
		Sleep(1000);
        EnemyBarTime = 0;
    }

	  if (BarTime >= 100) {
       MoveCursor(0, 21);
       std::cout << "Barra piena! Premi 'q' per attaccare!";
	}
	
        if (_kbhit()) {
            char Combat = _getch();
            if (Combat == 'q' && BarTime >= 100) {
                Entity.salute[EnemyID].HP -= Entity.attacco[PlayerID].Damage;
                
                MoveCursor(0, 21);
                std::cout << "Hai attaccato!                       ";
                
				Sleep(1000);
                BarTime = 0;
                

                if (Entity.salute[EnemyID].HP <= 0) {
                    Entity.EraseEntity(EnemyID);
                    system("cls");
                    MoveCursor(0, 1);
                    std::cout << "HAI VINTO!";
                    Sleep(1500);
                    
                    clean = false;
                    GameState = 0;
				} 
                MoveCursor(0, 25);
                std::cout << "                                        ";
            }
			
        }
		if(Entity.salute[PlayerID].HP <= 0){
				MoveCursor(0,1);
				system("cls");
				std::cout << "HAI PERSO!";
				Sleep(1500);
				GameState = 2;
				}
}

void GameInExecution(Map& m, EntityManager& Entity, EntityID& ID) {
    HideCursor();
	StartMenu Menu;
    bool InExecution = true;
    bool clean = false;
    int GameState = 0;
    EntityID CurrentEnemyID = -1;

    while (InExecution) {
        if (GameState == 0) {
           
            MoveCursor(0, 22);
            std::cout << "La tua salute: " << Entity.salute[ID].HP << " HP";
            
            if (_kbhit()) {
                char Tasto = _getch();
                if (Tasto == 27) {
					system("cls");
					Menu.SetAttivo(true);
                    while (Menu.GetAttivo()) {
                        Menu.MenuGraphic();
                        Menu.Input();
					 }
					system("cls");
				
                }else {
                    MuoviEntita(m, Entity, ID, Tasto);

                    EntityID hitEnemy = CollisionDetection(Entity, ID);
                    if (hitEnemy != -1) {
                        GameState = 1;
                        CurrentEnemyID = hitEnemy;
                    }
				}
			}
			MapRedering(m, Entity, ID);
            
        } else if (GameState == 1) {
            CombatSystem(Entity, ID, CurrentEnemyID, clean, GameState);
        }
		else if(GameState == 2){   // <----- risolvere il problema del bug de cursore che non si muove
			Menu.MenuLose();
			Menu.Input();
			Menu.SetAttivo(false);
		}
        Sleep(20);
       }
    }


/*
	#aggiungere la sconfitta se vieni sconfitto
	#mettere la classe animazione
	#aumentare la mappa
	#aggiungere redering della mappa
	
	*/