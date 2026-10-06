
#include "stdafx.h"

#include "Sistemi.h"
#include "Map.h"
#include "StartMenu.h"
#include "SlotManager.h"
#include "EntityManager.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <ctime>

#include "Windows.h"


int _tmain(
    int argc,
    _TCHAR* argv[]
)
{
    // =====================================================
    // RANDOM
    // =====================================================

    srand(
        (unsigned int)time(NULL)
    );


    // =====================================================
    // ENTITY MANAGER
    // =====================================================

    EntityManager Entity;


    // =====================================================
    // PLAYER
    // =====================================================

    EntityID PlayerID =
        Entity.CreateEntity(
            100,
            50,
            '@',
            2,
            10,
            100,
            2
        );


    Entity.livello[PlayerID].Exp =
        0;

    Entity.livello[PlayerID].level =
        0;

    Entity.livello[PlayerID].MaxExp =
        10;

    Entity.livello[PlayerID].PointStats =
        0;


    // =====================================================
    // SKILL PLAYER
    // =====================================================

    Entity.AddSkill(
        PlayerID,
        "Pugno",
        2
    );

    Entity.AddSkill(
        PlayerID,
        "Pugno_pesante",
        3
    );


    // =====================================================
    // SLIME 1
    // =====================================================

    EntityID Slime1 =
        Entity.CreateEntity(
            20,
            20,
            '&',
            2,
            8,
            100,
            3
        );

    Entity.livello[Slime1].Exp =
        3;

    Entity.entityrange[Slime1].AttackRangeX =
        12;

    Entity.entityrange[Slime1].AttackRangeY =
        12;


    // =====================================================
    // SLIME 2
    // =====================================================

    EntityID Slime2 =
        Entity.CreateEntity(
            40,
            30,
            '&',
            2,
            8,
            100,
            3
        );

    Entity.livello[Slime2].Exp =
        3;

    Entity.entityrange[Slime2].AttackRangeX =
        12;

    Entity.entityrange[Slime2].AttackRangeY =
        12;


    // =====================================================
    // SLIME 3
    // =====================================================

    EntityID Slime3 =
        Entity.CreateEntity(
            120,
            60,
            '&',
            2,
            8,
            100,
            3
        );

    Entity.livello[Slime3].Exp =
        3;

    Entity.entityrange[Slime3].AttackRangeX =
        12;

    Entity.entityrange[Slime3].AttackRangeY =
        12;


    // =====================================================
    // ORCO BOSS
    // =====================================================

    EntityID OrcBoss =
        Entity.CreateEntity(
            10,
            10,
            'O',
            6,
            30,
            100,
            1
        );

    Entity.livello[OrcBoss].Exp =
        20;


    // =====================================================
    // =====================================================

    Entity.posizione.erase(
        Slime1
    );

    Entity.posizione.erase(
        Slime2
    );

    Entity.posizione.erase(
        Slime3
    );

    Entity.posizione.erase(
        OrcBoss
    );


    // =====================================================
    // MAPPA
    // =====================================================

    Map MioMappa;


    // =====================================================
    // MENU / SLOT
    // =====================================================

    StartMenu Menu;

    SlotManager Slots;


    // =====================================================
    // CICLO PRINCIPALE
    // =====================================================

    while(true)
    {
        Menu.SetAttivo(
            true
        );

        while(
            Menu.GetAttivo()
        )
        {
            Menu.MenuGraphic();

            Menu.Input();
        }


        // =================================================
        // PLAY
        // =================================================

        if(
            Menu.GetChoice() == 2
        )
        {
            system("cls");

            Entity.visuale[PlayerID].Simbolo = '@';

            Slots.SetActive(
                true
            );

            while(
                Slots.GetActive()
            )
            {
                Slots.SlotMenu();

                Slots.SlotInput(
                    Entity,
                    PlayerID
                );

                Sleep(20);
            }


            // =============================================
            // ESC DAL MENU SLOT
            // =============================================

            if(
                Slots.GetChoice() == 0
            )
            {
                system("cls");

                continue;
            }


            // =============================================
            // SLOT SELEZIONATO
            // =============================================

            if(
                Slots.GetChoice() >= 1 &&
                Slots.GetChoice() <= 3
            )
            {
                // =================================================
                // SCELTA PERSONAGGIO
                // =================================================
                char ClasseCaricata =
                    Entity.visuale[PlayerID].Simbolo;

                if(
                    ClasseCaricata != 'N' &&
                    ClasseCaricata != 'C' &&
                    ClasseCaricata != 'M'
                )
                {
                    SelezionaPersonaggio(
                        Entity,
                        PlayerID
                    );

                    Slots.SaveData(
                        Entity,
                        PlayerID
                    );
                }


                if(
                    Entity.posizione.count(Slime1) == 0
                )
                {
                    SpawnNemicoCasuale(
                        MioMappa,
                        Entity,
                        PlayerID,
                        Slime1
                    );
                }

                if(
                    Entity.posizione.count(Slime2) == 0
                )
                {
                    SpawnNemicoCasuale(
                        MioMappa,
                        Entity,
                        PlayerID,
                        Slime2
                    );
                }

                if(
                    Entity.posizione.count(Slime3) == 0
                )
                {
                    SpawnNemicoCasuale(
                        MioMappa,
                        Entity,
                        PlayerID,
                        Slime3
                    );
                }

                system("cls");

                GameInExecution(
                    MioMappa,
                    Entity,
                    PlayerID,
                    Slots,
                    OrcBoss
                );
            }
        }
    }


    return 0;
}
