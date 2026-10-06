#include "stdafx.h"
#include "StatsGUI.h"

#include <iostream>
#include <conio.h>

#include "Windows.h"
#include "Sistemi.h"


StatsGUI::StatsGUI()
{
    isOpen = false;
}


// =====================================================
// INPUT
// =====================================================

void StatsGUI::Input(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    if(!_kbhit())
    {
        return;
    }

    char Tasto = _getch();


    // ESC oppure M -> chiudi statistiche
    if(
        Tasto == 27 ||
        Tasto == 'm' ||
        Tasto == 'M'
    )
    {
        isOpen = false;

        system("cls");

        return;
    }


    // nessun punto disponibile
    if(
        Entity.livello[PlayerID].PointStats <= 0
    )
    {
        return;
    }


    // ATTACCO
    if(Tasto == '1')
    {
        Entity.attacco[PlayerID].Damage++;

        Entity.livello[PlayerID].PointStats--;
    }


    // SALUTE MASSIMA
    else if(Tasto == '2')
    {
        Entity.salute[PlayerID].MaxHP++;

        Entity.salute[PlayerID].HP++;

        Entity.livello[PlayerID].PointStats--;
    }
}


// =====================================================
// GRAFICA
// =====================================================

void StatsGUI::StatsGraphic(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    // titolo
    SetConsoleTextAttribute(
        hConsole,
        11
    );

    MoveCursor(52, 5);

    std::cout
        << "PLAYER STATS";


    // box
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


    // punti disponibili
    SetConsoleTextAttribute(
        hConsole,
        14
    );

    MoveCursor(48, 9);

    std::cout
        << "STAT POINTS: "
        << Entity.livello[PlayerID].PointStats
        << "     ";


    // attacco
    SetConsoleTextAttribute(
        hConsole,
        15
    );

    MoveCursor(48, 12);

    std::cout
        << "[1] ATTACK";

    MoveCursor(67, 12);

    std::cout
        << Entity.attacco[PlayerID].Damage
        << "     ";


    // salute
    MoveCursor(48, 15);

    std::cout
        << "[2] HEALTH";

    MoveCursor(67, 15);

    std::cout
        << Entity.salute[PlayerID].HP
        << " / "
        << Entity.salute[PlayerID].MaxHP
        << "     ";


    // exp
    MoveCursor(48, 18);

    std::cout
        << "EXP";

    MoveCursor(67, 18);

    std::cout
        << Entity.livello[PlayerID].Exp
        << "     ";


    // comandi
    SetConsoleTextAttribute(
        hConsole,
        8
    );

    MoveCursor(45, 24);

    std::cout
        << "[1/2] UPGRADE";

    MoveCursor(65, 24);

    std::cout
        << "[M / ESC] BACK";
}


bool StatsGUI::GetisOpen()
{
    return isOpen;
}


void StatsGUI::SetisOpen(bool a)
{
    isOpen = a;
}