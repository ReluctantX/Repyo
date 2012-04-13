#include "StdAfx.h"
#include "Setup.h"
#include <vector>

vector< vector<uint32> > pvpKillStreakCounter(0);


int getPositionInList(uint32 pGUID);
void addPlayerToList(uint32 pGUID);
void deletePlayerFromList(uint32 pGUID);
void incKillstreakCounter(uint32 pGUID);
int getNumOfKills(uint32 pGUID);


void UwcEPvPKillstreakHandler(Player * pPlayer, Player * pVictim)
{
if(pPlayer->GetLowGUID() == pVictim->GetLowGUID()) return; // Suicide
char messageKillstreak[200] = "";
char messageKillstreakEnd[200];

// Increase Killstreak
if(getPositionInList(pPlayer->GetLowGUID()) == -1) addPlayerToList(pPlayer->GetLowGUID()); // Add to list
else incKillstreakCounter(pPlayer->GetLowGUID());

// Delete Victims Killstreak
if(getPositionInList(pVictim->GetLowGUID()) != -1)
{
        sprintf(messageKillstreakEnd, "|cffff0000[Killstreak System]|r |cffffffff%s has brought %s's killstreak to an end!|r", pPlayer->GetName(), pVictim->GetName(), pPlayer->CastSpell(pPlayer, 27239, true));
        sWorld.SendWorldWideScreenText(messageKillstreakEnd);
        deletePlayerFromList(pVictim->GetLowGUID());
       }
    
    if(getNumOfKills(pPlayer->GetLowGUID()) == 5) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 5|r]|r", pPlayer->GetName(), pVictim->GetName(), MSG_COLOR_LIGHTBLUE, pPlayer->CastSpell(pPlayer, 25796, true), pPlayer->CastSpell(pPlayer, 25799, true));
    
    else if(getNumOfKills(pPlayer->GetLowGUID()) == 15) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 15 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 58549, true)); // 15
    
      else if(getNumOfKills(pPlayer->GetLowGUID()) == 30) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 30 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 30

		 else if(getNumOfKills(pPlayer->GetLowGUID()) == 50) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 50 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 50 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 75) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 75 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 75 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 100) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 100 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 100 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 120) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 120 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 120 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 150) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 150 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 150 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 175) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 175 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 175 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 200) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 200 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 200 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 250) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 250 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 250 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 300) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 300 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 300 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 400) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 400 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 400 player kill streak

		else if(getNumOfKills(pPlayer->GetLowGUID()) == 500) sprintf(messageKillstreak, "|cffff0000[Killstreak System]|r |cffffffff %s is on a Killsteak of [|cffff0000 500 |r]|r", pPlayer->GetName(), pPlayer->CastSpell(pPlayer, 23505, true)); // 500 player kill streak

    
    if(messageKillstreak != "") sWorld.SendWorldWideScreenText(messageKillstreak);
}

void SetupAntiPvP(ScriptMgr * mgr)
{
    mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)UwcEPvPKillstreakHandler);
}

void addPlayerToList(uint32 pGUID)
{
    
    pvpKillStreakCounter.resize(pvpKillStreakCounter.size()+1);
    pvpKillStreakCounter[pvpKillStreakCounter.size()-1].resize(2);
    
    
    pvpKillStreakCounter[pvpKillStreakCounter.size()-1][0] = pGUID;
    pvpKillStreakCounter[pvpKillStreakCounter.size()-1][1] = (uint32) 1;
}

void deletePlayerFromList(uint32 pGUID)
{
    
    for(int i = getPositionInList(pGUID); (unsigned)i < (unsigned)pvpKillStreakCounter.size()-1; i++)
    {
        pvpKillStreakCounter[i][0] = pvpKillStreakCounter[i+1][0];
        pvpKillStreakCounter[i][1] = pvpKillStreakCounter[i+1][1];
    }
    
    
    pvpKillStreakCounter.resize(pvpKillStreakCounter.size()-1);
}

void incKillstreakCounter(uint32 pGUID)
{
    pvpKillStreakCounter[getPositionInList(pGUID)][1]++;
}

int getNumOfKills(uint32 pGUID)
{
    return (int)pvpKillStreakCounter[getPositionInList(pGUID)][1];
}


int getPositionInList(uint32 pGUID)
{
    for(int i = 0; (unsigned)i < (unsigned)pvpKillStreakCounter.size(); i++)
{
       if(pvpKillStreakCounter[i][0] == pGUID) return i;
   }
    
    
   return -1;
}