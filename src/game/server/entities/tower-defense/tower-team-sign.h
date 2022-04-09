#ifndef GAME_SERVER_ENTITIES_TDS_TOWER_H
#define GAME_SERVER_ENTITIES_TDS_TOWER_H

#include <game/server/entities/character.h>

class CTowerSign : public CEntity
{
public:
    CTowerSign(CGameWorld *pGameWorld, vec2 Pos, int Team, int Degres, int Dis);

    virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);
private:
    float m_Degres;
	int m_Distance;
    int m_Team;
    vec2 m_Pos;
    vec2 m_Pos1;
};

#endif