#include "stdafx.h"
#include "StartMenu.h"
#include <iostream>
#include <conio.h>
#include <cctype>

StartMenu::StartMenu(){
 OpzioneSelezionata = 0;
 Attivo = true;
}

void StartMenu::Input(){

	char Tasto = _getch();
	char TastoMaiuscolo = tolower(Tasto);
	if(TastoMaiuscolo == 'w'){
	OpzioneSelezionata = 1;
	}
	else if(TastoMaiuscolo == 's'){
	OpzioneSelezionata = 0;
	}
	else if(Tasto == 13){
		if(OpzioneSelezionata == 1){
		Attivo = false;
		}else{
		exit(0);
		}
	}
}

void StartMenu::MenuGraphic(){
	
	if(!Attivo){
	return;
	}
	system("cls");
	if(OpzioneSelezionata == 1){
		std::cout << ">Play" << std::endl;
	}	
	else{
		std::cout << " Play" << std::endl;
	}
	if(OpzioneSelezionata == 0){
		std::cout << ">Exit" << std::endl;
	}	
	else{
		std::cout << " Exit" << std::endl;
	}
}
int StartMenu::GetChoice(){
return OpzioneSelezionata;
}

void StartMenu::SetAttivo(bool a){
	Attivo = a;
}
bool StartMenu::GetAttivo(){
return Attivo;
}