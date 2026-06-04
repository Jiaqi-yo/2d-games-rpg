// prototipo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sistemi.h"
#include "Map.h"
#include <iostream>
#include <string>
#include <conio.h>


int _tmain(int argc, _TCHAR* argv[])
 {
	 Map MioMappa;
	char tasto = ' ';
	Posizione Player = {2, 2};

	MapRedering(MioMappa,Player);
    while (tasto != 'q') {
        tasto = _getch(); // Aspetta un tasto
	
	muoviEntita(Player, tasto);
	MapRedering(MioMappa,Player);

    }

    return 0;
}

