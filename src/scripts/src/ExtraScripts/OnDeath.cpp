#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32

#pragma warning(disable:4305)	

#endif#

void OnDeath(Player * Plr)
{
	Plr->BroadcastMessage("Revive function Available :");
	Plr->BroadcastMessage("Release Your character then use the teleporter to revive!");
	Plr->BroadcastMessage("*must be released or you'll end up bugged.");
}

void SetupOnDeath(ScriptMgr * mgr)
{
 mgr->register_hook(SERVER_HOOK_EVENT_ON_DEATH, OnDeath);
}