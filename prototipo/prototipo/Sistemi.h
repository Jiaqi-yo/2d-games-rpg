#ifndef SISTEMI_H
#define SISTEMI_H

#include "stdafx.h"
#include "Component.h"
#include "Map.h"
#include "EntityManager.h"

class SlotManager;

void SetupUnicodeConsole();

void MoveCursor(
    int x,
    int y
);

void HideCursor();

void SetColor(
    int color
);

void MuoviEntita(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    char Tasto
);

void DrawLegend();

void MapRedering(
    Map& m,
    EntityManager& Entity,
    EntityID& ID
);

int CollisionDetection(
    EntityManager& Entity,
    EntityID& ID
);

void AggiornaPlayer(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    int VecchiaX,
    int VecchiaY
);

void MuoviMostri(
    Map& m,
    EntityManager& Entity,
    EntityID& PlayerID,
    EntityID BossID
);

void SpawnNemicoCasuale(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID EnemyID
);

void GestisciRespawnNemici(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID BossID
);

void LevelUp(
    EntityManager& Entity,
    EntityID PlayerID
);

void CombatSystem(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID EnemyID,
    EntityID BossID,
    bool& clean,
    int& GameState,
    int& SlimeKills,
    bool& BossSpawned
);

void SelezionaPersonaggio(
    EntityManager& Entity,
    EntityID PlayerID
);

void MostraPersonaggio(
    EntityManager& Entity,
    EntityID PlayerID
);

void GameInExecution(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    SlotManager& Slots,
    EntityID BossID
);

#endif
