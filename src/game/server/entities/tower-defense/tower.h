#ifndef GAME_SERVER_ENTITIES_TD_TOWER_H
#define GAME_SERVER_ENTITIES_TD_TOWER_H

#include <game/server/entity.h>
#include "tower-team-sign.h"
#include <engine/shared/config.h>

//distance(pTarget->m_Pos, m_Pos) < 30
class CTower : public CEntity
{
public:
    CTower(CGameWorld *pGameWorld, vec2 Pos, int Team);

    CTowerSign *m_apTowerSigns;

    virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

    void TakeDamage(int Dmg);
    void LevelUpgrade();
    bool Warning();

private:
    int m_Health;
    int m_aIDs[9];
    int m_lIDs[9];
    int m_lIDs1[9];
    int m_lIDs2[100];
    int m_lIDs3[100];

    vec2 m_Pos;
    vec2 m_Pos1;

    int InTowerTick[MAX_CLIENTS];

    int m_Exp;
    int m_Level;
    int m_Team;
};

#endif