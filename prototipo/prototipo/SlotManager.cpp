#include "stdafx.h"
#include "SlotManager.h"
#include <conio.h>
#include "Windows.h"
#include <iostream>
#include "fstream"
#include "Vector"
#include "StartMenu.h"
#include <string>
#include "sistemi.h"

SlotManager::SlotManager(){
	getScelta = 0;
	Scelta  = 1;
	isActive = false;
}

void SlotManager::SlotInput(EntityManager& Entity,EntityID PlayerID){
	StartMenu Menu;
	if(_kbhit()){
	char Tasto = _getch();
	Tasto = tolower(Tasto);

	if(Tasto == 'w'){
	Scelta--;
	}
	if(Tasto == 's'){
	Scelta++;
	}

	if(Scelta < 1 ){
	Scelta = 3;
	}else if(Scelta > 3){
	Scelta = 1;
	}	
		
if(Tasto == 27){
system("cls");
Menu.MenuGraphic();
Menu.Input();
Menu.SetAttivo(true);
isActive = false;
}
	if(Scelta >= 1 || Scelta <= 3){
	
		std::string fileName = "";

		if(Tasto == 'x'){
			if(Scelta == 1){
				fileName = "Slot_1";
			}else if(Scelta == 2){
				fileName = "Slot_2";
			}else if(Scelta == 3){
				fileName = "Slot_3";
			}	
			if(std::remove(fileName.c_str()) == 0){
			std::cout << "Slot eliminato con successo!" << std::endl;
			Sleep(1000);
		}else{
			std::cout << "Nessun file di salvataggio trovato per lo slot" << std::endl;
			Sleep(1000);
		}
	}
}

	if(Tasto == 13 && Scelta == 1){
		std::cout << "caricamento dello slot";
		getScelta = 1;
		std::ifstream ControlFile("Slot_1");
		bool control = ControlFile.is_open();
		ControlFile.close();
		if(control){
			std::ifstream file("Slot_1");
			file >> Entity.visuale[PlayerID].Simbolo;
			file >> Entity.posizione[PlayerID].X;
			file >> Entity.posizione[PlayerID].Y;
			file >> Entity.salute[PlayerID].HP;
			file >> Entity.salute[PlayerID].MaxHP;
			file >> Entity.attacco[PlayerID].Damage;
			size_t Skills = 0;
			file >> Skills;
			Entity.skillset[PlayerID].skillset.clear();
			for(size_t i = 0 ; i <	Skills; i++){
				Skill LoadedSkill;
				file >> LoadedSkill.Danno;
				file >> LoadedSkill.Name;
				Entity.skillset[PlayerID].skillset.push_back(LoadedSkill);
				}
			file >> Entity.livello[PlayerID].Exp;
			file >> Entity.livello[PlayerID].MaxExp;
			file >> Entity.livello[PlayerID].level;
			file >> Entity.livello[PlayerID].PointStats;
			file.close();
			}else{
			getScelta = 1;
			std::ofstream file ("Slot_1");
			file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
			}	
	}else if(Tasto == 13 && Scelta == 2){
		std::cout << "caricamento dello slot";
		getScelta = 2;
		std::ifstream ControlFile("Slot_2");
		bool Control = ControlFile.is_open();
		ControlFile.close();
		if(Control){
			std::ifstream file("Slot_2");
			file >> Entity.visuale[PlayerID].Simbolo;
			file >> Entity.posizione[PlayerID].X;
			file >> Entity.posizione[PlayerID].Y;
			file >> Entity.salute[PlayerID].HP;
			file >> Entity.salute[PlayerID].MaxHP;
			file >> Entity.attacco[PlayerID].Damage;
			size_t Skills = 0;
			file >> Skills;
			Entity.skillset[PlayerID].skillset.clear();
			for(size_t i = 0 ; i <	Skills; i++){
				Skill LoadedSkill;
				file >> LoadedSkill.Danno;
				file >> LoadedSkill.Name;
				Entity.skillset[PlayerID].skillset.push_back(LoadedSkill);
				}
			file >> Entity.livello[PlayerID].Exp;
			file >> Entity.livello[PlayerID].MaxExp;
			file >> Entity.livello[PlayerID].level;
			file >> Entity.livello[PlayerID].PointStats;
			file.close();
		}else{
			getScelta = 2;
			std::ofstream file ("Slot_2");
			file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
			}
	}else if(Tasto == 13 && Scelta == 3){
		std::cout << "caricamento dello slot";
		getScelta = 3;
		std::ifstream ControlFile("Slot_3");
		bool Control = ControlFile.is_open();
		ControlFile.close();
		if(Control){std::ifstream file("Slot_3");
			file >> Entity.visuale[PlayerID].Simbolo;
			file >> Entity.posizione[PlayerID].X;
			file >> Entity.posizione[PlayerID].Y;
			file >> Entity.salute[PlayerID].HP;
			file >> Entity.salute[PlayerID].MaxHP;
			file >> Entity.attacco[PlayerID].Damage;
			size_t Skills = 0;
			file >> Skills;
			Entity.skillset[PlayerID].skillset.clear();
			for(size_t i = 0 ; i <	Skills; i++){
				Skill LoadedSkill;
				file >> LoadedSkill.Danno;
				file >> LoadedSkill.Name;
				Entity.skillset[PlayerID].skillset.push_back(LoadedSkill);
				}
			file >> Entity.livello[PlayerID].Exp;
			file >> Entity.livello[PlayerID].MaxExp;
			file >> Entity.livello[PlayerID].level;
			file >> Entity.livello[PlayerID].PointStats;
			file.close();
		}else{
			getScelta = 3;
			std::ofstream file ("Slot_3");
			file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
			}
		}
	}
}

void SlotManager::SaveData(EntityManager& Entity,EntityID PlayerID){
if(Scelta == 1){
	std::ofstream file("Slot_1");
	file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
	}else if(Scelta == 2){
	std::ofstream file("Slot_2");
	file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
	}else if(Scelta == 3){
	std::ofstream file("Slot_3");
	file << Entity.visuale[PlayerID].Simbolo << "\n"; 
			file << Entity.posizione[PlayerID].X << "\n";
			file << Entity.posizione[PlayerID].Y << "\n";
			file << Entity.salute[PlayerID].HP << "\n";
			file << Entity.salute[PlayerID].MaxHP << "\n";
			file << Entity.attacco[PlayerID].Damage << "\n";
			const std::vector<Skill>& skills = Entity.skillset[PlayerID].skillset;
			file << skills.size() << "\n";
			for(size_t i = 0;i < skills.size();i++){
				file << Entity.skillset[PlayerID].skillset[i].Danno << "\n";
				file << Entity.skillset[PlayerID].skillset[i].Name << "\n";
			}
			file << Entity.livello[PlayerID].Exp << "\n";
			file << Entity.livello[PlayerID].MaxExp << "\n";
			file << Entity.livello[PlayerID].level << "\n";
			file << Entity.livello[PlayerID].PointStats << "\n";
			file.close();
	}
}

void SlotManager::SlotMenu(){
	HideCursor();
	if(Scelta == 1){
		system("cls");
		MoveCursor(1,1);
		std::cout << "-->slot1" << std::endl;
	}else{
		MoveCursor(1,1);
		std::cout << "slot1" << std::endl;
	}
	if(Scelta == 2){
		system("cls");
		MoveCursor(2,2);
		std::cout << "-->slot2" << std::endl;
	}else{
		MoveCursor(2,2);
		std::cout << "slot2" << std::endl;
	}
	if(Scelta == 3){
		system("cls");
		MoveCursor(3,3);
		std::cout << "-->slot3" << std::endl;
	}else{
		MoveCursor(3,3);
		std::cout << "slot3" << std::endl;
	}
 
}
int SlotManager::GetChoice(){
	return getScelta;
}

void SlotManager::SetActive(bool s){
	isActive = s;
}

bool SlotManager::GetActive(){
	return isActive;
}
