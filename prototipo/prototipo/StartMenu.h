#ifndef STARTMENU_H
#define STARTMENU_H

class StartMenu
{
private:

    int OpzioneSelezionata;
    int OpzioneLose;
    bool Attivo;

public:

    StartMenu();

    void Input();
    void MenuGraphic();
    void Credits();

    void InputLose();
    void MenuLose();

    int GetChoice();

    void SetAttivo(bool a);

    bool GetAttivo();
};

#endif