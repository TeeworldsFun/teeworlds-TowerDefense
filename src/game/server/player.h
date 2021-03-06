/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_PLAYER_H
#define GAME_SERVER_PLAYER_H

// this include should perhaps be removed
#include "entities/character.h"
#include "gamecontext.h"
#include "Types.h"

// player object
class CPlayer
{
	MACRO_ALLOC_POOL_ID()

public:
	CPlayer(CGameContext *pGameServer, int ClientID, int Team);
	~CPlayer();

	void Init(int CID);

	void TryRespawn();
	void Respawn();
	void SetTeam(int Team, bool DoChatMsg=true);
	int GetTeam() const { return m_Team; };
	int GetCID() const { return m_ClientID; };

	void Tick();
	void PostTick();
	void Snap(int SnappingClient);

	void FakeSnap(int SnappingClient);
	
	void OnDirectInput(CNetObj_PlayerInput *NewInput);
	void OnPredictedInput(CNetObj_PlayerInput *NewInput);
	void OnDisconnect(const char *pReason);

	void KillCharacter(int Weapon = WEAPON_GAME);
	CCharacter *GetCharacter();

	const char* GetLanguage();
	void SetLanguage(const char* pLanguage);

	//---------------------------------------------------------
	// this is used for snapping so we know how we can clip the view for the player
	vec2 m_ViewPos;

	// states if the client is chatting, accessing a menu etc.
	int m_PlayerFlags;

	// used for snapping to just update latency if the scoreboard is active
	int m_aActLatency[MAX_CLIENTS];

	// used for spectator mode
	int m_SpectatorID;

	bool m_IsReady;

	//
	int m_Vote;
	int m_VotePos;
	//
	int m_LastVoteCall;
	int m_LastVoteTry;
	int m_LastChat;
	int m_LastSetTeam;
	int m_LastSetSpectatorMode;
	int m_LastChangeInfo;
	int m_LastEmote;
	int m_LastKill;

	// TODO: clean this up
	struct
	{
		char m_SkinName[64];
		int m_UseCustomColor;
		int m_ColorBody;
		int m_ColorFeet;
	} m_TeeInfos;

	int m_RespawnTick;
	int m_DieTick;
	int m_Score;
	int m_ScoreStartTick;
	bool m_ForceBalanced;
	int m_LastActionTick;
	int m_TeamChangeTick;
	struct
	{
		int m_TargetX;
		int m_TargetY;
	} m_LatestActivity;

	// network latency calculations
	struct
	{
		int m_Accum;
		int m_AccumMin;
		int m_AccumMax;
		int m_Avg;
		int m_Min;
		int m_Max;
	} m_Latency;

	int m_Authed;

	CTuningParams m_PrevTuningParams;
	CTuningParams m_NextTuningParams;

// Mine
public:
	void SendMineBroadcast(char* CraftType);
	bool FireMines(int Type);
	bool FireAmmo(int Type);
	void ChangeAmmoType();
	
	const char *GetMineName(int MineType)
	{
		switch (MineType)
		{
		case MINETYPE_COPPER:
			return "Copper";
			break;
		
		case MINETYPE_LEAD:
			return "Lead";
			break;

		case MINETYPE_COAL:
			return "Coal";
			break;
		
		default:
			break;
		}
	}

	const char *GetAmmoName(int AmmoType)
	{
		switch (AmmoType)
		{
		case AMMOTYPE_Dmg:
			return "Damage Ammo";
			break;
		
		case AMMOTYPE_Force:
			return "Force Ammo";
			break;

		case AMMOTYPE_Fire:
			return "Explosion Ammo";
			break;

		case AMMOTYPE_Slime:
			return "Slime Ammo";
			break;

		case AMMOTYPE_Freeze:
			return "Freeze Ammo";
			break;

		default:
			return "Normal";
			break;
		}
	}

	int GetMineNum(int MineType)
	{
		switch (MineType)
		{
		case MINETYPE_COPPER:
			return m_Copper;
			break;
		
		case MINETYPE_LEAD:
			return m_Lead;
			break;

		case MINETYPE_COAL:
			return m_Coal;
			break;
		
		default:
			break;
		}

		return 0;
	}

	int *GetMineNum_VL(int MineType)
	{
		switch (MineType)
		{
		case MINETYPE_COPPER:
			return &m_Copper;
			break;
		
		case MINETYPE_LEAD:
			return &m_Lead;
			break;

		case MINETYPE_COAL:
			return &m_Coal;
			break;
		
		default:
			break;
		}

		return 0;
	}

	int m_MineTick;
	int m_MiningType;
	
	bool m_InBase;
	
	int m_AmmoType;
private:
	CCharacter *m_pCharacter;
	CGameContext *m_pGameServer;

	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const;

	//
	bool m_Spawning;
	int m_ClientID;
	int m_Team;

	char m_aLanguage[16];

	void HandleTuningParams(); //This function will send the new parameters if needed

// Items
public:
	int m_Copper;
	int m_Lead;
	int m_Coal;

/*public:
	bool IsAttacker;
	int m_AttackerType; // 0 = not attacker.
	int m_BotMoveState;
	int m_AttackerMoveForce;*/
public:
	CTuningParams* GetNextTuningParams() { return &m_NextTuningParams; };
};

#endif
