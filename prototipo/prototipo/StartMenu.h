#include "stdafx.h"
#ifndef STARTMENU_H
#define STARTMENU_H
class StartMenu{

private:
	int getOption;
	int OpzioneSelezionata;
	int OpzioneLose;
	bool Attivo;

public:
	
	StartMenu();
	void Input();
	void MenuGraphic();
	void Credits();

	int InputLose();
	void MenuLose();

	int GetLose();
	int GetChoice();
	void SetAttivo(bool a);
	bool GetAttivo();
};

#endif