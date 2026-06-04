#include "stdafx.h"
#ifndef STARTMENU_H
#define STARTMENU_H
class StartMenu{

private:
	int OpzioneSelezionata;
	bool Attivo;

public:
	
	StartMenu();
	void Input();
	void MenuGraphic();
	int GetChoice();
	void SetAttivo(bool a);
	bool GetAttivo();
};

#endif