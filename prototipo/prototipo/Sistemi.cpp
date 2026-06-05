#include "stdafx.h"
#include "iostream"
#include "Sistemi.h"

void muoviEntita(Map& m,Posizione& p, char Tasto){
	if(Tasto == 'w'){
		if(m.GetCella(p.Y - 1,p.X)!= 1){
		p.Y--;
		}
	}
	if(Tasto == 's'){
		if(m.GetCella(p.Y + 1,p.X)!= 1){
		p.Y++;
		}
	} 
	if(Tasto == 'a'){
		if(m.GetCella(p.X - 1,p.Y)!= 1){
		p.X--;
		}
	}
	if(Tasto == 'd'){
		if(m.GetCella(p.X + 1,p.Y)!= 1){
		p.X++;
		}
	} 
}
void MapRedering(Map& m, Posizione& p){
	system("cls");
	for(int i = 0;i < 10; i++){
		for(int j = 0;j < 10; j++){
			if(i == p.Y && j == p.X){
				if(i == p.Y && j == p.X){
				std::cout << '@';
				}
			}else if(m.GetCella(i,j) == 1){
				std::cout << '#';
			}else if(m.GetCella(i,j) == 0){
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}