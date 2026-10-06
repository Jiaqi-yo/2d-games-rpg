#include "stdafx.h"
#include "SlotManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <cctype>
#include <cstdio>

#include "Windows.h"
#include "Sistemi.h"


// =====================================================
// CONTROLLO ESISTENZA SLOT
// =====================================================

bool SlotEsiste(const char* NomeFile)
{
    std::ifstream file(NomeFile);

    bool Esiste = file.is_open();

    file.close();

    return Esiste;
}


// =====================================================
// COSTRUTTORE
// =====================================================

SlotManager::SlotManager()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    getScelta = 0;
    Scelta = 1;

    isActive = false;
}


// =====================================================
// INPUT SLOT
// =====================================================

void SlotManager::SlotInput(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    if(!_kbhit())
    {
        return;
    }


    char Tasto = _getch();

    char TastoMinuscolo =
        tolower(Tasto);


    // =================================================
    // SU
    // =================================================

    if(TastoMinuscolo == 'w')
    {
        Scelta--;

        if(Scelta < 1)
        {
            Scelta = 3;
        }

        return;
    }


    // =================================================
    // GIU'
    // =================================================

    if(TastoMinuscolo == 's')
    {
        Scelta++;

        if(Scelta > 3)
        {
            Scelta = 1;
        }

        return;
    }


    // =================================================
    // ESC - TORNA AL MENU
    // =================================================

    if(Tasto == 27)
    {
        getScelta = 0;

        isActive = false;

        system("cls");

        return;
    }


    // =================================================
    // FILE DELLO SLOT SELEZIONATO
    // =================================================

    std::string fileName;


    if(Scelta == 1)
    {
        fileName = "Slot_1";
    }

    else if(Scelta == 2)
    {
        fileName = "Slot_2";
    }

    else
    {
        fileName = "Slot_3";
    }


    // =================================================
    // X - CANCELLA SLOT
    // =================================================

    if(TastoMinuscolo == 'x')
    {
        system("cls");


        HANDLE hConsole =
            GetStdHandle(STD_OUTPUT_HANDLE);


        if(
            std::remove(
                fileName.c_str()
            ) == 0
        )
        {
            SetConsoleTextAttribute(
                hConsole,
                10
            );

            MoveCursor(50, 14);

            std::cout
                << "SLOT "
                << Scelta
                << " ELIMINATO";
        }

        else
        {
            SetConsoleTextAttribute(
                hConsole,
                12
            );

            MoveCursor(48, 14);

            std::cout
                << "LO SLOT E' GIA' VUOTO";
        }


        Sleep(900);

        system("cls");

        return;
    }


    // =================================================
    // ENTER
    // =================================================

    if(Tasto == 13)
    {
        getScelta = Scelta;


        // =============================================
        // SLOT ESISTENTE -> CARICA
        // =============================================

        if(
            SlotEsiste(
                fileName.c_str()
            )
        )
        {
            std::ifstream file(
                fileName.c_str()
            );


            file
                >> Entity.visuale[PlayerID].Simbolo;


            file
                >> Entity.posizione[PlayerID].X;


            file
                >> Entity.posizione[PlayerID].Y;


            file
                >> Entity.salute[PlayerID].HP;


            file
                >> Entity.attacco[PlayerID].Damage;


            file
                >> Entity.livello[PlayerID].Exp;


            file.close();
        }


        // =============================================
        // SLOT VUOTO -> CREA NUOVO SALVATAGGIO
        // =============================================

        else
        {
            std::ofstream file(
                fileName.c_str()
            );


            file
                << Entity.visuale[PlayerID].Simbolo
                << "\n";


            file
                << Entity.posizione[PlayerID].X
                << "\n";


            file
                << Entity.posizione[PlayerID].Y
                << "\n";


            file
                << Entity.salute[PlayerID].HP
                << "\n";


            file
                << Entity.attacco[PlayerID].Damage
                << "\n";


            file
                << Entity.livello[PlayerID].Exp
                << "\n";


            file.close();
        }


        // esci dalla schermata slot
        isActive = false;

        system("cls");
    }
}


// =====================================================
// MENU SLOT
// =====================================================

void SlotManager::SlotMenu()
{
    HideCursor();


    HANDLE hConsole =
        GetStdHandle(
            STD_OUTPUT_HANDLE
        );


    // =====================================================
    // TITOLO
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(52, 3);

    std::cout
        << "ASCIIMYTHOS";


    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(53, 5);

    std::cout
        << "SELECT SAVE";


    // =====================================================
    // BOX PRINCIPALE
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(40, 7);

    std::cout
        << "\xE2\x95\x94";


    for(int i = 0; i < 44; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x97";


    for(int y = 8; y <= 20; y++)
    {
        MoveCursor(40, y);

        std::cout
            << "\xE2\x95\x91";


        MoveCursor(85, y);

        std::cout
            << "\xE2\x95\x91";
    }


    MoveCursor(40, 21);

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
    // DECORAZIONI
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(31, 10);

    std::cout
        << "\xE2\x96\x88"
        << " "
        << "\xE2\x96\x88";


    MoveCursor(30, 11);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    MoveCursor(31, 12);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(31, 13);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x88"
        << "\xE2\x96\x93";


    MoveCursor(30, 14);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    MoveCursor(92, 10);

    std::cout
        << "\xE2\x96\x88"
        << " "
        << "\xE2\x96\x88";


    MoveCursor(91, 11);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    MoveCursor(92, 12);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x91"
        << "\xE2\x96\x93";


    MoveCursor(92, 13);

    std::cout
        << "\xE2\x96\x93"
        << "\xE2\x96\x88"
        << "\xE2\x96\x93";


    MoveCursor(91, 14);

    std::cout
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88"
        << "\xE2\x96\x88";


    // =====================================================
    // SLOT 1
    // =====================================================

    MoveCursor(48, 10);


    if(Scelta == 1)
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << "> SLOT 1 <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "  SLOT 1  ";
    }


    MoveCursor(68, 10);


    if(SlotEsiste("Slot_1"))
    {
        SetConsoleTextAttribute(
            hConsole,
            10
        );

        std::cout
            << "[ SAVED ]";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            8
        );

        std::cout
            << "[ EMPTY ]";
    }


    // =====================================================
    // SLOT 2
    // =====================================================

    MoveCursor(48, 14);


    if(Scelta == 2)
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << "> SLOT 2 <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "  SLOT 2  ";
    }


    MoveCursor(68, 14);


    if(SlotEsiste("Slot_2"))
    {
        SetConsoleTextAttribute(
            hConsole,
            10
        );

        std::cout
            << "[ SAVED ]";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            8
        );

        std::cout
            << "[ EMPTY ]";
    }


    // =====================================================
    // SLOT 3
    // =====================================================

    MoveCursor(48, 18);


    if(Scelta == 3)
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << "> SLOT 3 <";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );

        std::cout
            << "  SLOT 3  ";
    }


    MoveCursor(68, 18);


    if(SlotEsiste("Slot_3"))
    {
        SetConsoleTextAttribute(
            hConsole,
            10
        );

        std::cout
            << "[ SAVED ]";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            8
        );

        std::cout
            << "[ EMPTY ]";
    }


    // =====================================================
    // COMANDI
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(39, 24);

    std::cout
        << "[ W / S ] MOVE";


    MoveCursor(60, 24);

    std::cout
        << "[ ENTER ] SELECT";


    MoveCursor(39, 26);

    std::cout
        << "[ X ] DELETE";


    MoveCursor(60, 26);

    std::cout
        << "[ ESC ] BACK";
}


// =====================================================
// SALVA PARTITA
// =====================================================

void SlotManager::SaveData(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    std::string fileName;


    if(getScelta == 1)
    {
        fileName = "Slot_1";
    }

    else if(getScelta == 2)
    {
        fileName = "Slot_2";
    }

    else if(getScelta == 3)
    {
        fileName = "Slot_3";
    }

    else
    {
        return;
    }


    std::ofstream file(
        fileName.c_str()
    );


    file
        << Entity.visuale[PlayerID].Simbolo
        << "\n";


    file
        << Entity.posizione[PlayerID].X
        << "\n";


    file
        << Entity.posizione[PlayerID].Y
        << "\n";


    file
        << Entity.salute[PlayerID].HP
        << "\n";


    file
        << Entity.attacco[PlayerID].Damage
        << "\n";


    file
        << Entity.livello[PlayerID].Exp
        << "\n";


    file.close();
}


// =====================================================
// GET CHOICE
// =====================================================

int SlotManager::GetChoice()
{
    return getScelta;
}


// =====================================================
// SET ACTIVE
// =====================================================

void SlotManager::SetActive(bool s)
{
    isActive = s;
}


// =====================================================
// GET ACTIVE
// =====================================================

bool SlotManager::GetActive()
{
    return isActive;
}