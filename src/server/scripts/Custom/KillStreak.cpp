/**************************************************************************************
*                                                                                     *
*                                                                                     *
*                                                                                     *
*                                                                                     *
*                                      _____                                          *
*                    *---------------*   |   *--------------*                         *
*                    |   Private     |*  |  *|     Edited   |                         *
*                     |    Gamers     |* | *|       By     |                          *
*                    |                 |* *|      Tommy     |                         *
*                   |-------------------|*|----------------|                          *
***************************************************************************************/
#include "ScriptPCH.h"

enum Spells
{
	SPELL_SPEED = 51442,
	SPELL_ENRAGE_REGEN = 55694,
	SPELL_MARK_WILD = 39233,
	SPELL_STAMINA = 48102
};

class System_OnKill : public PlayerScript
{
	public:
		System_OnKill() : PlayerScript("System_OnKill") {}

	void OnPVPKill(Player * Killer, Player * Victim)
	{
		uint64 KillerGUID = Killer->GetGUID();
		uint64 VictimGUID = Victim->GetGUID();

		struct KillStreak_Info
		{
			uint32 killstreak;
			uint64 lastkill;
		};

		static std::map<uint64, KillStreak_Info> KillStreakData;

		if( KillerGUID == VictimGUID || KillStreakData[KillerGUID].lastkill == VictimGUID )
			return;

		KillStreakData[KillerGUID].killstreak++;
		KillStreakData[KillerGUID].lastkill = VictimGUID;
		KillStreakData[VictimGUID].killstreak = 0;
		KillStreakData[VictimGUID].lastkill = 0;

		switch( KillStreakData[KillerGUID].killstreak )
		{
			char msg[250];
			case 10:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 20:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 30:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 40:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 50:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_SPEED, true);
				break;

			case 60:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_ENRAGE_REGEN, true);
				break;

			case 70:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 80:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 90:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 100:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_ENRAGE_REGEN, true);
				Killer->CastSpell(Killer, SPELL_MARK_WILD, true);
				break;

			case 110:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 120:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 130:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 140:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 150:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_ENRAGE_REGEN, true);
				break;

			case 160:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 170:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 180:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 190:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 200:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_ENRAGE_REGEN, true);
				Killer->CastSpell(Killer, SPELL_MARK_WILD, true);
				break;

			case 210:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 220:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_SPEED, true);
				break;

			case 230:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 240:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 250:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_SPEED, true);
				break;

			case 260:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 270:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 280:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 290:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;

			case 300:
				snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				Killer->CastSpell(Killer, SPELL_ENRAGE_REGEN, true);
				Killer->CastSpell(Killer, SPELL_STAMINA, true);
				break;
		}
	} 
};

void AddSC_PvP_System()
{
    new System_OnKill;
}