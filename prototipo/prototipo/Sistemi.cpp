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
		
	const int ViewWidth = 15;
	const int ViewHeight = 15;

	const int MapWidth = 20;
	const int MapHeight = 20;

	Posizione centerPos = Entity.posizione[ID];

	int StartX = centerPos.X - (ViewWidth / 2);
	int StartY = centerPos.Y - (ViewHeight / 2);

	if(StartX < 0){
	StartX = 0;
	}
	if(StartY < 0){
	StartY = 0;
	}

	if(StartX > MapWidth - ViewWidth){
		StartX = MapWidth - ViewWidth;
	}
	if(StartY > MapHeight - ViewWidth){
		StartY = MapHeight - ViewWidth;
	}
	
	for(int i = 0;i < ViewHeight; i++){
		MoveCursor(49, 0 + i);
		int WorldY = StartY + i;
		for(int j = 0;j < ViewWidth; j++){
			int WorldX = StartX + j;
			bool EntityFound = false;

			for(std::map<EntityID,Posizione>::iterator IT = Entity.posizione.begin(); IT != Entity.posizione.end(); ++IT){
			EntityID Id = IT->first;
			Posizione posizione = IT->second;
			if(IT->second.Y == WorldY && IT->second.X == WorldX){
			std::cout << Entity.visuale[IT->first].Simbolo;
			EntityFound = true;
			break;
			}
		}
			if(!EntityFound){
				if(WorldX >= 0 && WorldX < MapWidth && WorldY >= 0 && WorldY < MapHeight){
					int Cella = m.GetCella(WorldY,WorldX);
					if(Cella == 1){
						std::cout << '#';
					}else if (Cella == 0){
						std::cout << '.';
					}
				}else{
					std::cout << ' ';
				
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
void ResetGame(Map& m, EntityManager& Entity, EntityID& PlayerID, int& GameState,bool& clean) {

	Entity.salute[PlayerID].HP = Entity.salute[PlayerID].MaxHP;

    Entity.posizione[PlayerID].X = 1; 
    Entity.posizione[PlayerID].Y = 1;

	for(auto IT = Entity.posizione.begin(); IT != Entity.posizione.end(); ++IT){
	EntityID id = IT->first;
	
	if(id == PlayerID) continue;
	Entity.salute[id].HP = Entity.salute[id].MaxHP;
	}

	clean = false;
    GameState = 0;
    system("cls");	
}

void MobRespawn(EntityManager& Entity){
	
	for(EntityID EnemyID = 0;EnemyID < Entity.RespawnTImer.size(); EnemyID++){
	if(Entity.RespawnTImer[EnemyID].timer > 0){
	
		Entity.RespawnTImer[EnemyID].timer--;
		if(Entity.RespawnTImer[EnemyID].timer == 0){
		
		Entity.salute[EnemyID].HP = Entity.salute[EnemyID].MaxHP;
		if(Entity.atb[EnemyID].Time <= 0){
		Entity.atb[EnemyID].Time = 0;
		}

		Entity.posizione[EnemyID].X = Entity.RespawnTImer[EnemyID].ReX;
		Entity.posizione[EnemyID].Y = Entity.RespawnTImer[EnemyID].ReY;
			}
		}
	}
}


void CombatSystem(EntityManager& Entity, EntityID PlayerID, EntityID EnemyID, bool& clean, int& GameState) {
SetConsoleOutputCP(CP_UTF8);
    static int BarTime = 0;
    static int EnemyBarTime = 0;
    static bool inSceltaMosse = false; 

    if (!clean) {
        system("cls");
        clean = true;
        BarTime = 0;
        EnemyBarTime = 0;
        inSceltaMosse = false;
    }

    if (!inSceltaMosse) {
        if (BarTime < 100){ 
            BarTime += Entity.atb[PlayerID].Time;
            if(BarTime >= 100) BarTime = 100;
        }
        if (EnemyBarTime < 100){
            EnemyBarTime += Entity.atb[EnemyID].Time;
            if(EnemyBarTime >= 100) EnemyBarTime = 100;
        }
    }

    Sleep(200);

    MoveCursor(0, 22);
    std::cout << "La tua salute:" << Entity.salute[PlayerID].HP << " HP  ";

    MoveCursor(50, 22);
    std::cout << "Salute del nemico:" << Entity.salute[EnemyID].HP << "HP  ";
    
    MoveCursor(40, 20);
    std::cout << BLOCCO_PIENO;
    MoveCursor(40, 21);
    std::cout << BLOCCO_PIENO;
    MoveCursor(40, 22);
    std::cout << BLOCCO_PIENO;
    MoveCursor(40, 23);
    std::cout << BLOCCO_PIENO;
    MoveCursor(40, 24);
    std::cout << BLOCCO_PIENO;
  




    MoveCursor(0, 21);
    std::cout << "Caricamento della barra:" << BarTime << "%            ";

    MoveCursor(50, 21);
    std::cout << "Caricamento Nemico:" << EnemyBarTime << "%     ";


    if (!inSceltaMosse && EnemyBarTime >= 100) {
        Sleep(100);
        Entity.salute[PlayerID].HP -= Entity.attacco[EnemyID].Damage;
        
        MoveCursor(50, 21);
        std::cout << "Il nemico ti ha attaccato!";
        Sleep(1000);
        EnemyBarTime = 0;
    }

 	if (BarTime >= 100 && !inSceltaMosse) {
    MoveCursor(0, 21);
    std::cout << "Barra piena! Premi 'q' per selezionare!";
    }


    if (_kbhit()) {
        char Combat = _getch();
        

        if (!inSceltaMosse && Combat == 'q' && BarTime >= 100) {
            inSceltaMosse = true;
            MoveCursor(0, 21);
            std::cout << "Seleziona mossa!                        ";
			Sleep(1000);
            
			const auto& ListaMosse = Entity.skillset[PlayerID].skillset; 
            MoveCursor(0, 30);
            for (size_t i = 0; i < ListaMosse.size(); i++) {
                std::cout << "[" << (i + 1) << "]" << ListaMosse[i].Name << " (" << ListaMosse[i].Danno << ") danni" << std::endl;
            }
        }
        else if (inSceltaMosse && Combat >= '1' && Combat <= '4') {
            int IndiceMosse = Combat - '1';
			const auto& ListaMosse = Entity.skillset[PlayerID].skillset; 
            
            if (IndiceMosse < ListaMosse.size()) {
                Skill MossaScelta = ListaMosse[IndiceMosse];
                
                Entity.salute[EnemyID].HP -= MossaScelta.Danno;
                
                MoveCursor(0, 25);
                std::cout << "                                                                 ";

                MoveCursor(0, 25);
                std::cout << "Hai usato " << MossaScelta.Name << "! inflitti: " << MossaScelta.Danno;
                Sleep(1500);
				MoveCursor(0,25);
				std::cout << "                                                  ";

                if (Entity.salute[EnemyID].HP <= 0) {
					Entity.posizione.erase(EnemyID);
					Entity.RespawnTImer[EnemyID].timer = 250;

					Entity.livello[EnemyID].Exp;
                    system("cls");
                    MoveCursor(0, 1);
                    std::cout << "HAI VINTO!";
                    Sleep(1500);
                    
                    clean = false;
                    system("cls");
                    GameState = 0;
                }
                
                inSceltaMosse = false;
                BarTime = 0;
            }
        }
    }

    if (Entity.salute[PlayerID].HP <= 0) {
        MoveCursor(0, 1);
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
           
			MobRespawn(Entity);

            MoveCursor(0, 22);
            std::cout << "La tua salute: " << Entity.salute[ID].HP << " HP";
			MoveCursor(0,23);
			std::cout << "il tuo Livello:" << Entity.livello[ID].level;
			MoveCursor(0,24);
			std::cout << "exp:" << Entity.livello[ID].Exp << " su " << "[" << Entity.livello[ID].MaxExp << "] MaxExp";
			
            
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
		else if(GameState == 2){  
			Menu.MenuLose();
			int Scelta = Menu.InputLose();
			if(Scelta == 1){
			ResetGame(m,Entity,ID,GameState, clean);
			}
			if(Scelta == 0){
			exit(0);
			}
		}
        Sleep(20);
       }
    }


/*
	#aggiungere stats

	*/