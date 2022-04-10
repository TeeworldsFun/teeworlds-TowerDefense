#include "tower.h"
#include <game/server/gamecontext.h>
#include <new>

#define PickupPhysSizeS 14
#define M_PI 3.14159265358979323846
CTower::CTower(CGameWorld *pGameWorld, vec2 Pos, int Team)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_TOWER)
{
    m_Exp = 2;
    m_Level = 1;
    m_Pos = Pos;
    m_Pos1 = Pos;
    m_Team = Team;
    
    for (unsigned i = 0; i < sizeof(m_aIDs) / sizeof(int); i ++)
        m_aIDs[i] = Server()->SnapNewID();

    for (unsigned i = 0; i < sizeof(m_lIDs) / sizeof(int); i ++)
        m_lIDs[i] = Server()->SnapNewID();

    for (unsigned i = 0; i < sizeof(m_lIDs1) / sizeof(int); i ++)
        m_lIDs1[i] = Server()->SnapNewID();
    
    for (unsigned i = 0; i < sizeof(m_lIDs2) / sizeof(int); i ++)
        m_lIDs2[i] = Server()->SnapNewID();
    
    for (unsigned i = 0; i < sizeof(m_lIDs3) / sizeof(int); i ++)
        m_lIDs3[i] = Server()->SnapNewID();

    m_ProximityRadius = PickupPhysSizeS;

    GameWorld()->InsertEntity(this);

    for(int i = 0;i < MAX_LEVEL;i++)
    new CTowerSign(pGameWorld, Pos, Team, i, 200);
}

void CTower::LevelUpgrade()
{
    if(m_Exp == m_Level*2)
    {
        m_Exp -= m_Level*2;
        m_Level++;
    }
}

void CTower::Reset() 
{
    GameServer()->m_World.DestroyEntity(this);
	for (unsigned i = 0; i < sizeof(m_aIDs) / sizeof(int); i ++)
	{
		if(m_aIDs[i] >= 0){
			Server()->SnapFreeID(m_aIDs[i]);
			m_aIDs[i] = -1;
		}
	}

    for (unsigned i = 0; i < sizeof(m_lIDs) / sizeof(int); i ++)
	{
		if(m_lIDs[i] >= 0){
			Server()->SnapFreeID(m_lIDs[i]);
			m_lIDs[i] = -1;
		}
	}
    
    for (unsigned i = 0; i < sizeof(m_lIDs1) / sizeof(int); i ++)
	{
		if(m_lIDs1[i] >= 0){
			Server()->SnapFreeID(m_lIDs1[i]);
			m_lIDs1[i] = -1;
		}
	}

    for (unsigned i = 0; i < sizeof(m_lIDs2) / sizeof(int); i ++)
	{
		if(m_lIDs2[i] >= 0){
			Server()->SnapFreeID(m_lIDs2[i]);
			m_lIDs2[i] = -1;
		}
	}

    for (unsigned i = 0; i < sizeof(m_lIDs3) / sizeof(int); i ++)
	{
		if(m_lIDs3[i] >= 0){
			Server()->SnapFreeID(m_lIDs3[i]);
			m_lIDs3[i] = -1;
		}
	}
}

void CTower::Tick()
{
    for(int i = 0;i < MAX_CLIENTS; i++)
    {
        if(!GameServer()->m_apPlayers[i])
            return;
        
        if(!GameServer()->m_apPlayers[i]->GetCharacter())
            return;

        if(distance(GameServer()->m_apPlayers[i]->GetCharacter()->m_Pos, m_Pos) < 200)
        {
            InTowerTick[i]++;
            if(InTowerTick[i]%150 == 0)
            {
                InTowerTick[i] = 0;
                
            }
        }
    }
    LevelUpgrade();
}

void CTower::Snap(int SnappingClient)
{
    int aSize = sizeof(m_aIDs) / sizeof(int);
    int lSize = sizeof(m_lIDs) / sizeof(int);
    int lSize1 = sizeof(m_lIDs1) / sizeof(int);
    int lSize2 = sizeof(m_lIDs2) / sizeof(int);
    int lSize3 = sizeof(m_lIDs3) / sizeof(int);

    for (int i = 0; i < aSize; i ++) 
	{
        CNetObj_Projectile *pEff = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_aIDs[i], sizeof(CNetObj_Projectile)));
		if (!pEff)
            continue;

		pEff->m_X = (int)(cos(M_PI/9*i*4)*(16.0+5/110)+m_Pos.x);
		pEff->m_Y = (int)(sin(M_PI/9*i*4)*(16.0+5/110)+m_Pos.y);

		pEff->m_StartTick = Server()->Tick()-2;
		pEff->m_Type = WEAPON_SHOTGUN;
    }

    for(int i = 0; i < lSize; i++)
    {
        CNetObj_Laser *pLaser = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_lIDs[i], sizeof(CNetObj_Laser)));
        if(!pLaser)
            return;

        pLaser->m_X = (int)(cos(M_PI/9*i*4)*(100.0+5/110*i)+m_Pos.x);
		pLaser->m_Y = (int)(sin(M_PI/9*i*4)*(100.0+5/110*i)+m_Pos.y);
    }

    for(int i = 0; i < lSize1; i++)
    {
        CNetObj_Laser *pLaser1 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_lIDs1[i], sizeof(CNetObj_Laser)));
        if(!pLaser1)
            return;

        pLaser1->m_X = (int)(cos(M_PI/9*i*4)*(80.0+2.5/50*i)+m_Pos.x);
		pLaser1->m_Y = (int)(sin(M_PI/9*i*4)*(80.0+2.5/50*i)+m_Pos.y);
    }

    for(int i = 0; i < lSize2; i++)
    {
        CNetObj_Laser *pLaser2 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_lIDs2[i], sizeof(CNetObj_Laser)));
        if(!pLaser2)
            return;

        pLaser2->m_X = (int)(cos(3.15/150*i*4)*(200.0+5/50*i)+m_Pos.x);
		pLaser2->m_Y = (int)(sin(3.15/150*i*4)*(200.0+5/50*i)+m_Pos.y);
    }

    for(int i = 0; i < lSize3; i++)
    {
        CNetObj_Laser *pLaser3 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_lIDs3[i], sizeof(CNetObj_Laser)));
        if(!pLaser3)
            return;

        pLaser3->m_X = (int)(cos(3.15/40*i*4)*(190.0+5/50*i)+m_Pos.x);
		pLaser3->m_Y = (int)(sin(3.15/40*i*4)*(190.0+5/50*i)+m_Pos.y);
        pLaser3->m_FromX = m_Pos.x;
        pLaser3->m_FromY = m_Pos.y;
        pLaser3->m_StartTick = Server()->Tick()-6;
    }

    CNetObj_Flag *pFlag = static_cast<CNetObj_Flag *>(Server()->SnapNewItem(NETOBJTYPE_FLAG, m_ID, sizeof(CNetObj_Flag)));
    if(!pFlag)
        return;

    pFlag->m_Team = TEAM_BLUE;
    pFlag->m_X = m_Pos.x;
    pFlag->m_Y = m_Pos.y;
}