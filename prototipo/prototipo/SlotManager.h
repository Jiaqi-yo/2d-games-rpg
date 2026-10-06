#ifndef SLOTMANAGER_H
#define SLOTMANAGER_H

#include "EntityManager.h"

class SlotManager
{
private:

    bool isActive;

    int getScelta;
    int Scelta;

public:

    SlotManager();

    void SlotInput(
        EntityManager& Entity,
        EntityID PlayerID
    );

    void SlotMenu();

    void SaveData(
        EntityManager& Entity,
        EntityID PlayerID
    );

    int GetChoice();

    void SetActive(bool s);

    bool GetActive();
};

#endif