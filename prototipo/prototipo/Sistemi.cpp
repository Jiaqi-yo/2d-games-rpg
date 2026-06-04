#include "stdafx.h"
#include "iostream"
#include "Sistemi.h"

void muoviEntita(Posizione& p, char Tasto){
	if(Tasto == 'w') p.Y--;
	if(Tasto == 's') p.Y++;
	if(Tasto == 'a') p.X--;
	if(Tasto == 'd') p.X++;
}


void MapRedering(Map& m, Posizione& p){
	system("cls");
	for(int i = 0;i < 10; i++){
		for(int j = 0;j < 10; j++){
			if(i == p.Y && j == p.X){
				if(i == p.Y,j == p.X){
				std::cout << '@';
				}
			}else if(m.GetCella(i,j) == 1){
				std::cout << '#';
			}else{
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}