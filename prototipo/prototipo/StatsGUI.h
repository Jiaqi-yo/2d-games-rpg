#ifndef StatsGUI_H
#define StatsGUI_H

#include "EntityManager.h"

class StatsGUI
{
private:

    bool isOpen;

public:

    StatsGUI();

    void Input(
        EntityManager& Entity,
        EntityID PlayerID
    );

    void StatsGraphic(
        EntityManager& Entity,
        EntityID PlayerID
    );

    bool GetisOpen();

    void SetisOpen(bool a);
};

#endif