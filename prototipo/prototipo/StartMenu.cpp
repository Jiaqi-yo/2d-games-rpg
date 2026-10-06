#include "stdafx.h"
#include "StartMenu.h"

#include <iostream>
#include <conio.h>
#include <cctype>
#include <cstdlib>

#include "Windows.h"
#include "Sistemi.h"


// =====================================================
// COSTRUTTORE
// =====================================================

StartMenu::StartMenu()
{
    // UTF-8 attivo fin dal menu
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    OpzioneSelezionata = 2;
    OpzioneLose = 1;
    Attivo = true;
}


// =====================================================
// INPUT MENU PRINCIPALE
// =====================================================

void StartMenu::Input()
{
    char Tasto = _getch();

    char TastoMinuscolo =
        tolower(Tasto);


    // SU
    if(TastoMinuscolo == 'w')
    {
        OpzioneSelezionata++;

        // loop circolare
        if(OpzioneSelezionata > 2)
        {
            OpzioneSelezionata = 0;
        }
    }


    // GIU'
    else if(TastoMinuscolo == 's')
    {
        OpzioneSelezionata--;

        // loop circolare
        if(OpzioneSelezionata < 0)
        {
            OpzioneSelezionata = 2;
        }
    }


    // INVIO
    else if(Tasto == 13)
    {
        // EXIT
        if(OpzioneSelezionata == 0)
        {
            exit(0);
        }


        // CREDITS
        else if(OpzioneSelezionata == 1)
        {
            system("cls");

            Credits();

            MoveCursor(42, 26);

            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                8
            );

            std::cout
                << "Premi un tasto per tornare al menu";

            _getch();

            system("cls");
        }


        // PLAY
        else if(OpzioneSelezionata == 2)
        {
            Attivo = false;
        }
    }
}


// =====================================================
// MENU PRINCIPALE
// =====================================================

void StartMenu::MenuGraphic()
{
    HideCursor();

    if(!Attivo)
    {
        return;
    }


    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    // =====================================================
    // TITOLO ASCIIMYTHOS
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(27, 2);

    std::cout
        << "     _     ____   ____ ___ ___ __  __ __   __ _____ _   _  ___  ____  ";


    MoveCursor(27, 3);

    std::cout
        << "    / \\   / ___| / ___|_ _|_ _|  \\/  |\\ \\ / /|_   _| | | |/ _ \\/ ___| ";


    MoveCursor(27, 4);

    std::cout
        << "   / _ \\  \\___ \\| |    | | | || |\\/| | \\ V /   | | | |_| | | | \\___ \\ ";


    MoveCursor(27, 5);

    std::cout
        << "  / ___ \\  ___) | |___ | | | || |  | |  | |    | | |  _  | |_| |___) |";


    MoveCursor(27, 6);

    std::cout
        << " /_/   \\_\\|____/ \\____|___|___|_|  |_|  |_|    |_| |_| |_|\\___/|____/ ";


    // =====================================================
    // SOTTOTITOLO
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );

    MoveCursor(49, 8);

    std::cout
        << "AN ASCII FANTASY RPG";


    // =====================================================
    // TORRE SINISTRA
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(28, 11);

    std::cout
        << " "
        << "\xE2\x96\x88"
        << " "
        << "\xE2\x96\x88"
        << " ";


    MoveCursor(27, 12);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    MoveCursor(28, 13);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(28, 14);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x88"
        << "\xE2\x96\x93";


    MoveCursor(28, 15);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(27, 16);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    // =====================================================
    // TORRE DESTRA
    // =====================================================

    MoveCursor(93, 11);

    std::cout
        << " "
        << "\xE2\x96\x88"
        << " "
        << "\xE2\x96\x88"
        << " ";


    MoveCursor(92, 12);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    MoveCursor(93, 13);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(93, 14);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x88"
        << "\xE2\x96\x93";


    MoveCursor(93, 15);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(92, 16);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    // =====================================================
    // BOX MENU
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(42, 10);

    std::cout
        << "\xE2\x95\x94";


    for(int i = 0; i < 40; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x97";


    for(int y = 11; y <= 21; y++)
    {
        MoveCursor(42, y);

        std::cout
            << "\xE2\x95\x91";


        MoveCursor(83, y);

        std::cout
            << "\xE2\x95\x91";
    }


    MoveCursor(42, 22);

    std::cout
        << "\xE2\x95\x9A";


    for(int i = 0; i < 40; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x9D";


    // =====================================================
    // DECORAZIONI INTERNE
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(47, 12);

    std::cout
        << "\xE2\x97\x86"
        << "                            "
        << "\xE2\x97\x86";


    MoveCursor(47, 20);

    std::cout
        << "\xE2\x97\x86"
        << "                            "
        << "\xE2\x97\x86";


    // =====================================================
    // PLAY
    // =====================================================

    MoveCursor(57, 13);


    if(OpzioneSelezionata == 2)
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << ">  PLAY  <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "   PLAY   ";
    }


    // =====================================================
    // CREDITS
    // =====================================================

    MoveCursor(55, 16);


    if(OpzioneSelezionata == 1)
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << ">  CREDITS  <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "   CREDITS   ";
    }


    // =====================================================
    // EXIT
    // =====================================================

    MoveCursor(57, 19);


    if(OpzioneSelezionata == 0)
    {
        SetConsoleTextAttribute(
            hConsole,
            12
        );

        std::cout
            << ">  EXIT  <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "   EXIT   ";
    }


    // =====================================================
    // DECORAZIONI ESTERNE
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        10
    );


    MoveCursor(34, 18);

    std::cout
        << "\xE2\x99\xA3";


    MoveCursor(37, 20);

    std::cout
        << "\xE2\x99\xA3";


    MoveCursor(88, 18);

    std::cout
        << "\xE2\x99\xA3";


    MoveCursor(86, 20);

    std::cout
        << "\xE2\x99\xA3";


    SetConsoleTextAttribute(
        hConsole,
        7
    );


    MoveCursor(32, 21);

    std::cout
        << "\xE2\x97\x86";


    MoveCursor(90, 21);

    std::cout
        << "\xE2\x97\x86";


    // =====================================================
    // CONTROLLI
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(45, 25);

    std::cout
        << "[ W / S ] MOVE";


    MoveCursor(66, 25);

    std::cout
        << "[ ENTER ] SELECT";


    // =====================================================
    // VERSIONE
    // =====================================================

    MoveCursor(28, 28);

    std::cout
        << "ASCIIMYTHOS - DEVELOPMENT BUILD 0.1";
}


// =====================================================
// CREDITS
// =====================================================

void StartMenu::Credits()
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(53, 4);

    std::cout
        << "ASCIIMYTHOS";


    MoveCursor(55, 6);

    std::cout
        << "CREDITS";


    // =====================================================
    // BOX CREDITS
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(40, 8);

    std::cout
        << "\xE2\x95\x94";


    for(int i = 0; i < 44; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x97";


    for(int y = 9; y <= 22; y++)
    {
        MoveCursor(40, y);

        std::cout
            << "\xE2\x95\x91";


        MoveCursor(85, y);

        std::cout
            << "\xE2\x95\x91";
    }


    MoveCursor(40, 23);

    std::cout
        << "\xE2\x95\x9A";


    for(int i = 0; i < 44; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x9D";


    // =====================================================
    // CHEN JIAQI
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        14
    );


    MoveCursor(54, 11);

    std::cout
        << "Chen Jiaqi";


    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(49, 13);

    std::cout
        << "Game Logic & Combat Systems";


    // =====================================================
    // JOVAN LAZAROV
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(53, 16);

    std::cout
        << "Jovan Lazarov";


    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(49, 18);

    std::cout
        << "Game Design & Visual Design";


    // =====================================================
    // VERSIONE
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(53, 21);

    std::cout
        << "ASCIIMYTHOS 0.1";
}


// =====================================================
// INPUT GAME OVER
// =====================================================

void StartMenu::InputLose()
{
    char Tasto = _getch();

    char TastoMinuscolo =
        tolower(Tasto);


    if(TastoMinuscolo == 'w')
    {
        OpzioneLose = 1;
    }

    else if(TastoMinuscolo == 's')
    {
        OpzioneLose = 0;
    }
}


// =====================================================
// MENU GAME OVER
// =====================================================

void StartMenu::MenuLose()
{
    system("cls");


    if(OpzioneLose == 1)
    {
        std::cout
            << "> Restart"
            << std::endl;
    }
    else
    {
        std::cout
            << "  Restart"
            << std::endl;
    }


    if(OpzioneLose == 0)
    {
        std::cout
            << "> Exit"
            << std::endl;
    }
    else
    {
        std::cout
            << "  Exit"
            << std::endl;
    }
}


// =====================================================
// GET CHOICE
// =====================================================

int StartMenu::GetChoice()
{
    return OpzioneSelezionata;
}


// =====================================================
// SET ATTIVO
// =====================================================

void StartMenu::SetAttivo(
    bool a
)
{
    Attivo = a;
}


// =====================================================
// GET ATTIVO
// =====================================================

bool StartMenu::GetAttivo()
{
    return Attivo;
}