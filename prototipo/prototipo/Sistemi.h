#include "stdafx.h"
#include "Component.h"
#include "Map.h"
#include "EntityManager.h"
#ifndef SISTEMI_H
#define SISTEMI_H


void MuoviEntita(Map& m,EntityManager& Entity,EntityID& ID,char Tasto);
void MapRedering(Map& m, EntityManager& Entity,EntityID& ID);
void GameInExecution(Map& m, EntityManager& Entity, EntityID& ID);
int CollisionDetection(EntityManager& Entity, EntityID& ID);
void CombatSystem(EntityManager& Entity, EntityID& Enemy);
void MoveCursor(int x,int y);
void HideCursor();



#endif