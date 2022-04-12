#ifndef GAME_SERVER_ENTITIES_TD_ATTACKER_H
#define GAME_SERVER_ENTITIES_TD_ATTACKER_H

#include <game/server/entities/character.h>
#include <game/server/entity.h>
#include <game/generated/server_data.h>
#include <game/generated/protocol.h>

#include <game/gamecore.h>

// Dagger
class CAttacker : public CCharacter
{
	MACRO_ALLOC_POOL_ID()

public:
    CAttacker(CGameWorld *pWorld);
    static const int ms_PhysSize = 28;

	virtual void Tick();
	virtual bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon);

private:
	vec2 m_BotLastPos;
    int m_BotDir;
    int m_BotStuckCount;
    int m_BotClientIDFix;
    float m_BotLastStuckTime;
    float m_BotTimePlayerFound;
    float m_BotTimeGrounded;
    float m_BotTimeLastOption;
    float m_BotTimeLastDamage;
    float m_BotTimeLastSound;
    bool m_BotJumpTry;

	void TickBotAI();
	void PlaySound();
	void RunAction();
};

#endif
