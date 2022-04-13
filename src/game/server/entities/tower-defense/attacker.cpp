/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <new>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/mapitems.h>
#include "attacker.h"
#include "../character.h"
#include "../projectile.h"

MACRO_ALLOC_POOL_ID_IMPL(CAttacker, MAX_CLIENTS)

CAttacker::CAttacker(CGameWorld *pWorld)
: CCharacter(pWorld)
{
	m_BotDir = 1;
	m_BotLastPos = m_Pos;
	m_BotLastStuckTime = 0.0f;
	m_BotStuckCount = 0;
	m_BotTimePlayerFound = Server()->Tick();
	m_BotTimeGrounded = Server()->Tick();
	m_BotTimeLastOption = Server()->Tick();
	m_BotTimeLastDamage = 0.0f;
	m_BotClientIDFix = -1;
	m_BotTimeLastSound = Server()->Tick();
	m_BotJumpTry = false;
	m_BotMoveState = MoveType_Left;
}

void CAttacker::Tick()
{
    if(GameServer()->m_pController->IsGameOver())
        return;
    TickBotAI();
	CCharacter::Tick();
}

bool CAttacker::TakeDamage(vec2 Force, int Dmg, int From, int Weapon)
{
	if (CCharacter::TakeDamage(Force, Dmg, From, Weapon))
	{
		m_BotTimeLastDamage = Server()->Tick();
		return true;
	}

	return false;
}

void CAttacker::PlaySound()
{

}

void CAttacker::RunAction()
{
	if (m_BotClientIDFix != -1 && GameServer()->m_apPlayers[m_BotClientIDFix])
	{
		CCharacter *pChar = GameServer()->m_apPlayers[m_BotClientIDFix]->GetCharacter();
		if (!pChar)
		{
			m_BotClientIDFix = -1;
			return;
		}

		if (Server()->Tick() - m_BotTimePlayerFound > 0)
		{
			m_LatestInput.m_Fire = m_Input.m_Fire = 1;
			m_BotDir = 0;
			m_BotTimePlayerFound = Server()->Tick();
		}
        
		m_BotClientIDFix = -1;
	}
}

void CAttacker::TickBotAI()
{
    CTuningParams* pTuningParams = &GetPlayer()->m_NextTuningParams;

    //Sounds
    if (Server()->Tick() - m_BotTimeLastSound > Server()->TickSpeed()*5.0f && !(rand()%50))
    {
        PlaySound();
        m_BotTimeLastSound = Server()->Tick();
    }

    //Clean m_Input
	m_Input.m_Fire = 0;

    //Run actions
    RunAction();

    //Interact with users
    bool PlayerClose = false;
    bool PlayerFound = false;
    bool TowerFound = false;

    int DistTower = distance(GameServer()->m_TowerPos, m_Pos);
    if(DistTower <= 500)
    {
        TowerFound = true;
        vec2 DirPlayer = normalize(GameServer()->m_TowerPos - m_Pos);
        if (DirPlayer.x < 0)
            m_BotDir = -1;
        else
            m_BotDir = 1;
        if(m_aWeapons[WEAPON_GUN].m_Ammo)
        {
            m_ActiveWeapon = WEAPON_GUN;
            m_aWeapons[WEAPON_GUN].m_Ammo++;
        }
        
        m_BotClientIDFix = GetPlayer()->GetCID();

        m_Input.m_TargetX = static_cast<int>(GameServer()->m_TowerPos.x - m_Pos.x);
        m_Input.m_TargetY = static_cast<int>(GameServer()->m_TowerPos.y - m_Pos.y);

        m_LatestInput.m_Fire = m_Input.m_Fire = 1;
    }
    else
        TowerFound = false;
    float LessDist = 500.0f;

    m_BotClientIDFix = -1;
    
	for (int i=0; i<MAX_CLIENTS; i++)
	{
	    CPlayer *pPlayer = GameServer()->m_apPlayers[i];
	    if (!pPlayer || !pPlayer->GetCharacter() || pPlayer->IsAttacker)
            continue;
        
        int Dist = distance(pPlayer->GetCharacter()->m_Pos, m_Pos);
        if (Dist < LessDist)
            LessDist = Dist;
        else
            continue;

        if (Dist < 320.0f && !GameServer()->Collision()->IntersectLine(pPlayer->GetCharacter()->m_Pos, m_Pos, NULL, NULL) && !TowerFound)
        {
            vec2 DirPlayer = normalize(pPlayer->GetCharacter()->m_Pos - m_Pos);
            if (DirPlayer.x < 0)
                m_BotDir = -1;
            else
                m_BotDir = 1;
        
            if(m_aWeapons[WEAPON_GUN].m_Ammo)
            {
                m_ActiveWeapon = WEAPON_GUN;
                m_aWeapons[WEAPON_GUN].m_Ammo++;
            }
            
            PlayerClose = true;
            m_BotClientIDFix = pPlayer->GetCID();
    
            m_Input.m_TargetX = static_cast<int>(pPlayer->GetCharacter()->m_Pos.x - m_Pos.x);
            m_Input.m_TargetY = static_cast<int>(pPlayer->GetCharacter()->m_Pos.y - m_Pos.y);

            PlayerFound = true;
        }


    }
    //Fix target
    if (!PlayerFound)
    {
        m_Input.m_TargetX = m_BotDir;
        m_Input.m_TargetY = 0;
    }

    m_EmoteType = EMOTE_ANGRY;

    
    //Random Actions
	if (!PlayerFound && Server()->Tick()-m_BotTimeLastOption > Server()->TickSpeed()*10.0f)
	{
        int Action = rand()%3;
        if (Action == 0)
                m_BotDir = -1;
        else if (Action == 1)
                m_BotDir = 1;
        else if (Action == 2)
            m_BotDir = 0;

        GameServer()->SendEmoticon(GetPlayer()->GetCID(), 2+rand()%2);	
        m_BotTimeLastOption = Server()->Tick();
    
	}

    //Limits
    int tx = m_Pos.x+m_BotDir*45.0f;
    if (tx < 0)
        m_BotDir = 1;
    else if (tx >= GameServer()->Collision()->GetWidth()*32.0f)
    	m_BotDir = -1;

    //Delay of actions
    if (!PlayerClose)
        m_BotTimePlayerFound = Server()->Tick()-5;

    //Set data
    m_Input.m_Direction = m_BotDir;
	m_Input.m_PlayerFlags = PLAYERFLAG_PLAYING;
	//Check for legacy input
	if (m_LatestPrevInput.m_Fire && m_Input.m_Fire) m_Input.m_Fire = 0;

	m_LatestPrevInput = m_LatestInput;
	m_LatestInput = m_Input;
	m_BotLastPos = m_Pos;

	pTuningParams->m_GroundControlAccel = 0.0f;
	pTuningParams->m_GroundJumpImpulse = 0.0f;
	pTuningParams->m_AirJumpImpulse = 0.0f;
	pTuningParams->m_AirControlAccel = 0.0f;
	pTuningParams->m_Gravity = 0.0f;
    pTuningParams->m_AirControlAccel = 0.0f;
    pTuningParams->m_GroundControlAccel = 0.0f;
	CCharacter::FireWeapon();
}
