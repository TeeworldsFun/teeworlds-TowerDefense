#include <new>
#include "tower-team-sign.h"
#include <game/server/gamecontext.h>

#define M_PIl 3.141592653589793238462643383279502884L
#define PickupPhysSizeS 28

CTowerSign::CTowerSign(CGameWorld *pGameWorld, vec2 Pos, int Team, int Degres, int Dis)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
    m_Degres = 360/MAX_LEVEL*Degres;
    m_Distance = Dis;
	m_ProximityRadius = PickupPhysSizeS;
    m_Team = Team;
    m_Pos = Pos;
    m_Pos1 = Pos;
	GameWorld()->InsertEntity(this);
}

void CTowerSign::Reset()
{
    GameServer()->m_World.DestroyEntity(this);
    Server()->SnapFreeID(m_ID);
}

void CTowerSign::Tick()
{
	m_Pos = GameServer()->m_TowerPos[m_Team];
	m_Pos1 = GameServer()->m_TowerPos[m_Team];
	m_Pos = m_Pos1 + (GetDir(m_Degres*M_PIl/180) * m_Distance);
	if ( m_Degres + 1 < 360 )
		m_Degres += 1;
	else
		m_Degres = 0;
}

void CTowerSign::Snap(int SnappingClient)
{
	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_Pos.x;
	pP->m_Y = (int)m_Pos.y;

	if(m_Team == TEAM_RED)
    	pP->m_Type = POWERUP_HEALTH;
	else
        pP->m_Type = POWERUP_ARMOR;
	pP->m_Subtype = 0;
}