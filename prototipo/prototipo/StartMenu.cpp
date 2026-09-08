 #include "stdafx.h"
#include "StartMenu.h"
#include <iostream>
#include <conio.h>
#include <cctype>
#include "Windows.h"
#include "Sistemi.h"


StartMenu::StartMenu(){
 OpzioneSelezionata = 1;
 OpzioneLose = 1;
 Attivo = true;
}
void StartMenu::Input(){
	char Tasto = _getch();
	char TastoMinuscolo = tolower(Tasto);
	
	if(TastoMinuscolo == 'w'){
	OpzioneSelezionata++;
}
	if(TastoMinuscolo == 's'){
	OpzioneSelezionata--;
	}

	if(OpzioneSelezionata < 0){
	OpzioneSelezionata = 2;
	}else if(OpzioneSelezionata > 2){
	OpzioneSelezionata = 0;
	}
	
	if(Tasto == 13){
		if(OpzioneSelezionata == 0){
		exit(0);
		}else if(OpzioneSelezionata == 1){
		system("cls");
		Credits();
		std::cout << "clicca qualsiasi tasto per continuare" << std::endl;
		_getch();
		system("cls");
		}else if(OpzioneSelezionata == 2){
		Attivo = false;
		}
	}
}

void StartMenu::MenuGraphic(){
	 HideCursor();
	if(!Attivo){
	return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole,11);
	MoveCursor(28, 1);
	std::cout << "    _    ____   ____ ___ ___ __  __ __   __ _____ _   _  ___  ____  ";
	MoveCursor(28, 2);
	std::cout << "   / \\  / ___| / ___|_ _|_ _|  \\/  |\\ \\ / /|_   _| | | |/ _ \\/ ___| ";
	MoveCursor(28, 3);
	std::cout << "  / _ \\ \\___ \\| |    | | | || |\\/| | \\ V /   | | | |_| | | | \\___ \\ ";
	MoveCursor(28, 4);
	std::cout << " / ___ \\ ___) | |___ | | | || |  | |  | |    | | |  _  | |_| |___) |";
	MoveCursor(28, 5);
	std::cout << "/_/   \\_\\____/ \\____|___|___|_|  |_|  |_|    |_| |_| |_|\\___/|____/ ";


		

	if(OpzioneSelezionata == 2){
	SetConsoleTextAttribute(hConsole,12);	
	}	
	else{
	SetConsoleTextAttribute(hConsole,15);	
	}

		MoveCursor(48, 10); std::cout << " ____  _        _   __   __ ";
MoveCursor(48, 11); std::cout << "|  _ \\| |      / \\  \\ \\ / / ";
MoveCursor(48, 12); std::cout << "| |_) | |     / _ \\  \\ V /  ";
MoveCursor(48, 13); std::cout << "|  __/| |___ / ___ \\  | |   ";
MoveCursor(48, 14); std::cout << "|_|   |_____/_/   \\_\\ |_|   ";

if(OpzioneSelezionata == 1){
	SetConsoleTextAttribute(hConsole,12);
}else{
	SetConsoleTextAttribute(hConsole,15);
}

MoveCursor(33, 16); std::cout << "  ____    ____     _____   ____    ___   _____   ____  ";
MoveCursor(33, 17); std::cout << " / ___|  |  _ \\   | ____| |  _ \\  |_ _| |_   _| / ___| ";
MoveCursor(33, 18); std::cout << " | |     | |_) |  |  _|   | | | |  | |    | |   \\___ \\ ";
MoveCursor(33, 19); std::cout << " | |___  |  _ <   | |___  | |_| |  | |    | |    ___) |";
MoveCursor(33, 20); std::cout << "  \\____| |_| \\_\\  |_____| |____/  |___|   |_|   |____/ ";


	if(OpzioneSelezionata == 0){
	SetConsoleTextAttribute(hConsole,12);	
	}	
	else{
		SetConsoleTextAttribute(hConsole,15);	
	}
MoveCursor(46, 22); std::cout << " _____   __  __   ___   _____ ";
MoveCursor(46, 23); std::cout << "| ____|  \\ \\/ /  |_ _| |_   _|";
MoveCursor(46, 24); std::cout << "|  _|     \\  /    | |    | |  ";
MoveCursor(46, 25); std::cout << "| |___    /  \\    | |    | |  ";
MoveCursor(46, 26); std::cout << "|_____|  /_/\\_\\  |___|   |_|  ";
}

int StartMenu::InputLose(){
	char Tasto = _getch();
    char TastoMinuscolo = tolower(Tasto);
    
    if (TastoMinuscolo == 'w') {
        OpzioneLose = 1;
    }
    else if (TastoMinuscolo == 's') {
        OpzioneLose = 0; 
    }
    else if (Tasto == 13) {
        return OpzioneLose; 
    }
    
    return -1;
}

void StartMenu::MenuLose(){

	system("cls");
	if(OpzioneLose == 1){
		std::cout << ">Restart" << std::endl;
	}
	else{
		std::cout << "Restart" << std::endl;
	}
	if(OpzioneLose == 0){
		std::cout << ">Exit" << std::endl;
	}
	else{
		std::cout << "Exit" << std::endl;
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

void StartMenu::Credits(){
	std::cout << "Chen Jiaqi" << std::endl;
	std::cout << "Capo del Progetto" << std::endl;
}

int StartMenu::GetLose(){
return OpzioneLose;
}
