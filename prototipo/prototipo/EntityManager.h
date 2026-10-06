#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "stdafx.h"
#include <string>
#include <map>
#include "Component.h"
#include "Windows.h"

class EntityManager
{
private:
    int NextID;

public:
    std::map<EntityID, Posizione> posizione;
    std::map<EntityID, Attacco> attacco;
    std::map<EntityID, Salute> salute;
    std::map<EntityID, Livello> livello;
    std::map<EntityID, Visuale> visuale;
    std::map<EntityID, ATB> atb;

    std::map<EntityID, EntityRespawn> RespawnTImer;
    std::map<EntityID, Skillset> skillset;
    std::map<EntityID, EntityRange> entityrange;

    EntityManager()
    {
        NextID = 0;
    }

    EntityID CreaID()
    {
        return NextID++;
    }

    void EraseEntity(EntityID ID)
    {
        posizione.erase(ID);
        attacco.erase(ID);
        salute.erase(ID);
        livello.erase(ID);
        visuale.erase(ID);
        atb.erase(ID);
        RespawnTImer.erase(ID);
        skillset.erase(ID);
        entityrange.erase(ID);
    }

    void AddSkill(
        EntityID id,
        std::string name,
        int danno
    )
    {
        Skill NewSkill;

        NewSkill.Name = name;
        NewSkill.Danno = danno;

        skillset[id].skillset.push_back(
            NewSkill
        );

        skillset[id].skillImparate =
            (int)skillset[id].skillset.size();
    }

    EntityID CreateEntity(
        int x,
        int y,
        char simbolo,
        double att,
        double health,
        int bt,
        int tm
    )
    {
        EntityID NuovoID =
            CreaID();

        Posizione pos;
        pos.X = x;
        pos.Y = y;
        posizione[NuovoID] = pos;

        Visuale vis;
        vis.Simbolo = simbolo;
        visuale[NuovoID] = vis;

        Attacco damage;
        damage.Damage = att;
        attacco[NuovoID] = damage;

        Salute sal;
        sal.HP = health;
        sal.MaxHP = health;
        salute[NuovoID] = sal;

        Livello liv;
        liv.Exp = 0;
        liv.level = 0;
        liv.MaxExp = 10;
        liv.PointStats = 0;
        livello[NuovoID] = liv;

        ATB time;
        time.BarTime = bt;
        time.Time = tm;
        atb[NuovoID] = time;

        Skillset set;
        set.skillImparate = 0;
        skillset[NuovoID] = set;

        EntityRespawn respawn;
        respawn.ReX = x;
        respawn.ReY = y;
        respawn.timer = 0;
        RespawnTImer[NuovoID] = respawn;

        EntityRange range;
        range.AttackRangeX = 5;
        range.AttackRangeY = 5;
        entityrange[NuovoID] = range;

        return NuovoID;
    }
};

#endif
