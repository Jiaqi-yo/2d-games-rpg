// prototipo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sistemi.h"
#include "Map.h"
#include "StartMenu.h"
#include <iostream>
#include <string>
#include <conio.h>


int _tmain(int argc, _TCHAR* argv[])
 {
	 Map MioMappa;
	 StartMenu Menu;
	 char tasto = ' ';
	 Posizione Player = {2, 2};
	 bool GameInExecution = true;
	 while(GameInExecution){
		 if(Menu.GetAttivo()){
			 Menu.MenuGraphic();
			 Menu.Input();
		 }else{
			MapRedering(MioMappa,Player);
			tasto = _getch();

			if(tasto == 27){
				Menu.SetAttivo(true);
			}else{
			muoviEntita(MioMappa,Player, tasto);
			MapRedering(MioMappa,Player);
			}
	}
}
    return 0;
}

