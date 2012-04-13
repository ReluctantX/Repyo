/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

initialiseSingleton(ChatHandler);
initialiseSingleton(CommandTableStorage);

ChatCommand* ChatHandler::getCommandTable()
{
	ARCEMU_ASSERT(false);
	return 0;
}

ChatCommand* CommandTableStorage::GetSubCommandTable(const char* name)
{
	if(!stricmp(name, "modify"))
		return _modifyCommandTable;
	else if(!stricmp(name, "waypoint"))
		return _waypointCommandTable;
	else if(!stricmp(name, "debug"))
		return _debugCommandTable;
	else if(!stricmp(name, "gmTicket"))
		return _GMTicketCommandTable;
	else if(!stricmp(name, "gobject"))
		return _GameObjectCommandTable;
	else if(!stricmp(name, "battleground"))
		return _BattlegroundCommandTable;
	else if(!stricmp(name, "npc"))
		return _NPCCommandTable;
	else if(!stricmp(name, "cheat"))
		return _CheatCommandTable;
	else if(!stricmp(name, "account"))
		return _accountCommandTable;
	else if(!stricmp(name, "honor"))
		return _honorCommandTable;
	else if(!stricmp(name, "quest"))
		return _questCommandTable;
	else if(!stricmp(name, "pet"))
		return _petCommandTable;
	else if(!stricmp(name, "recall"))
		return _recallCommandTable;
	else if(!stricmp(name, "guild"))
		return _GuildCommandTable;
	else if(!stricmp(name, "gm"))
		return _gmCommandTable;
	else if(!stricmp(name, "server"))
		return _serverCommandTable;
	else if(!stricmp(name, "character"))
		return _characterCommandTable;
	else if(!stricmp(name, "lookup"))
		return _lookupCommandTable;
	else if(!stricmp(name, "admin"))
		return _adminCommandTable;
	else if(!stricmp(name, "kick"))
		return _kickCommandTable;
	else if(!stricmp(name, "ban"))
		return _banCommandTable;
	else if(!stricmp(name, "unban"))
		return _unbanCommandTable;
	else if(!stricmp(name, "instance"))
		return _instanceCommandTable;
	else if(!stricmp(name, "arena"))
		return _arenaCommandTable;
	else if(!stricmp(name, "achieve"))
		return _achievementCommandTable;
	else if(!stricmp(name, "vehicle"))
		return _vehicleCommandTable;
	return 0;
}

#define dupe_command_table(ct, dt) this->dt = (ChatCommand*)allocate_and_copy(sizeof(ct)/* / sizeof(ct[0])*/, ct)
ARCEMU_INLINE void* allocate_and_copy(uint32 len, void* pointer)
{
	void* data = (void*)malloc(len);
	memcpy(data, pointer, len);
	return data;
}

void CommandTableStorage::Load()
{
	QueryResult* result = WorldDatabase.Query("SELECT * FROM command_overrides");
	if(!result) return;

	do
	{
		const char* name = result->Fetch()[0].GetString();
		const char* level = result->Fetch()[1].GetString();
		Override(name, level);
	}
	while(result->NextRow());
	delete result;
}

void CommandTableStorage::Override(const char* command, const char* level)
{
	ARCEMU_ASSERT(level[0] != '\0');
	char* cmd = strdup(command);

	// find the command we're talking about
	char* sp = strchr(cmd, ' ');
	const char* command_name = cmd;
	const char* subcommand_name = NULL;

	if(sp != NULL)
	{
		// we're dealing with a subcommand.
		*sp = 0;
		subcommand_name = sp + 1;
	}

	size_t len1 = strlen(command_name);
	size_t len2 = subcommand_name ? strlen(subcommand_name) : 0;

	// look for the command.
	ChatCommand* p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(!strnicmp(p->Name, command_name, len1))
		{
			// this is the one we wanna modify
			if(!subcommand_name)
			{
				// no subcommand, we can change it.
				p->CommandGroup = level[0];
				LOG_DETAIL("Changing command level of command `%s` to %c.", p->Name, level[0]);
			}
			else
			{
				// assume this is a subcommand, loop the second set.
				ChatCommand* p2 = p->ChildCommands;
				if(!p2)
				{
					LOG_ERROR("Invalid command specified for override: %s", command_name);
				}
				else
				{
					while(p2->Name != 0)
					{
						if(!strnicmp("*", subcommand_name, 1))
						{
							p2->CommandGroup = level[0];
							LOG_DETAIL("Changing command level of command (wildcard) `%s`:`%s` to %c.", p->Name, p2->Name, level[0]);
						}
						else
						{
							if(!strnicmp(p2->Name, subcommand_name, len2))
							{
								// change the level
								p2->CommandGroup = level[0];
								LOG_DETAIL("Changing command level of command `%s`:`%s` to %c.", p->Name, p2->Name, level[0]);
								break;
							}
						}
						p2++;
					}
					if(p2->Name == 0)
					{
						if(strnicmp("*", subcommand_name, 1)) //Hacky.. meh.. -DGM
						{
							LOG_ERROR("Invalid subcommand referenced: `%s` under `%s`.", subcommand_name, p->Name);
						}
						break;
					}
				}
			}
			break;
		}
		++p;
	}

	if(p->Name == 0)
	{
		LOG_ERROR("Invalid command referenced: `%s`", command_name);
	}

	free(cmd);
}

void CommandTableStorage::Dealloc()
{
	free(_modifyCommandTable);
	free(_debugCommandTable);
	free(_waypointCommandTable);
	free(_GMTicketCommandTable);
	free(_GuildCommandTable);
	free(_GameObjectCommandTable);
	free(_BattlegroundCommandTable);
	free(_NPCCommandTable);
	free(_CheatCommandTable);
	free(_accountCommandTable);
	free(_honorCommandTable);
	free(_petCommandTable);
	free(_recallCommandTable);
	free(_questCommandTable);
	free(_serverCommandTable);
	free(_gmCommandTable);
	free(_characterCommandTable);
	free(_lookupCommandTable);
	free(_adminCommandTable);
	free(_kickCommandTable);
	free(_banCommandTable);
	free(_unbanCommandTable);
	free(_instanceCommandTable);
	free(_arenaCommandTable);
	free(_achievementCommandTable);
	free(_vehicleCommandTable);
	free(_commandTable);
}

void CommandTableStorage::Init()
{
	static ChatCommand modifyCommandTable[] =
	{
		{ "hp",              '8', NULL,                                   "Modifies health points (HP) of selected target",                  NULL, UNIT_FIELD_HEALTH,                 UNIT_FIELD_MAXHEALTH, 1 },
		{ "hp",              '9', NULL,                                   "Modifies health points (HP) of selected target",                  NULL, UNIT_FIELD_HEALTH,                 UNIT_FIELD_MAXHEALTH, 1 },
		{ "gender",          '8', &ChatHandler::HandleGenderChanger,      "Changes gender of selected target. Usage: 0=male, 1=female.",     NULL, 0,                                 0,                    0 },
		{ "gender",          '9', &ChatHandler::HandleGenderChanger,      "Changes gender of selected target. Usage: 0=male, 1=female.",     NULL, 0,                                 0,                    0 },
		{ "mana",            '8', NULL,                                   "Modifies mana points (MP) of selected target.",                   NULL, UNIT_FIELD_POWER1,                 UNIT_FIELD_MAXPOWER1, 1 },
		{ "mana",            '9', NULL,                                   "Modifies mana points (MP) of selected target.",                   NULL, UNIT_FIELD_POWER1,                 UNIT_FIELD_MAXPOWER1, 1 },
		{ "rage",            '8', NULL,                                   "Modifies rage points of selected target.",                        NULL, UNIT_FIELD_POWER2,                 UNIT_FIELD_MAXPOWER2, 1 },
		{ "rage",            '9', NULL,                                   "Modifies rage points of selected target.",                        NULL, UNIT_FIELD_POWER2,                 UNIT_FIELD_MAXPOWER2, 1 },
		{ "energy",          '8', NULL,                                   "Modifies energy points of selected target.",                      NULL, UNIT_FIELD_POWER4,                 UNIT_FIELD_MAXPOWER4, 1 },
		{ "energy",          '9', NULL,                                   "Modifies energy points of selected target.",                      NULL, UNIT_FIELD_POWER4,                 UNIT_FIELD_MAXPOWER4, 1 },
		{ "runicpower",      '8', NULL,                                   "Modifies runic power points of selected target.",                 NULL, UNIT_FIELD_POWER7,                 UNIT_FIELD_MAXPOWER7, 1 },
		{ "runicpower",      '9', NULL,                                   "Modifies runic power points of selected target.",                 NULL, UNIT_FIELD_POWER7,                 UNIT_FIELD_MAXPOWER7, 1 },
		{ "level",           '8', &ChatHandler::HandleModifyLevelCommand, "Modifies the level of selected target.",                          NULL, 0,                                 0,                    0 },
		{ "level",           '9', &ChatHandler::HandleModifyLevelCommand, "Modifies the level of selected target.",                          NULL, 0,                                 0,                    0 },
		{ "strength",        '8', NULL,                                   "Modifies the strength value of the selected target.",             NULL, UNIT_FIELD_STAT0,                  0,                    1 },
		{ "strength",        '9', NULL,                                   "Modifies the strength value of the selected target.",             NULL, UNIT_FIELD_STAT0,                  0,                    1 },
		{ "agility",         '8', NULL,                                   "Modifies the agility value of the selected target.",              NULL, UNIT_FIELD_STAT1,                  0,                    1 },
		{ "agility",         '9', NULL,                                   "Modifies the agility value of the selected target.",              NULL, UNIT_FIELD_STAT1,                  0,                    1 },
		{ "intelligence",    '8', NULL,                                   "Modifies the intelligence value of the selected target.",         NULL, UNIT_FIELD_STAT3,                  0,                    1 },
		{ "intelligence",    '9', NULL,                                   "Modifies the intelligence value of the selected target.",         NULL, UNIT_FIELD_STAT3,                  0,                    1 },
		{ "spirit",          '8', NULL,                                   "Modifies the spirit value of the selected target.",               NULL, UNIT_FIELD_STAT4,                  0,                    1 },
		{ "spirit",          '9', NULL,                                   "Modifies the spirit value of the selected target.",               NULL, UNIT_FIELD_STAT4,                  0,                    1 },
		{ "armor",           '8', NULL,                                   "Modifies the armor of selected target.",                          NULL, UNIT_FIELD_RESISTANCES,            0,                    1 },
		{ "armor",           '9', NULL,                                   "Modifies the armor of selected target.",                          NULL, UNIT_FIELD_RESISTANCES,            0,                    1 },
		{ "holy",            '8', NULL,                                   "Modifies the holy resistance of selected target.",                NULL, UNIT_FIELD_RESISTANCES + 1,         0,                    1 },
		{ "holy",            '9', NULL,                                   "Modifies the holy resistance of selected target.",                NULL, UNIT_FIELD_RESISTANCES + 1,         0,                    1 },
		{ "fire",            '8', NULL,                                   "Modifies the fire resistance of selected target.",                NULL, UNIT_FIELD_RESISTANCES + 2,         0,                    1 },
		{ "fire",            '9', NULL,                                   "Modifies the fire resistance of selected target.",                NULL, UNIT_FIELD_RESISTANCES + 2,         0,                    1 },
		{ "nature",          '8', NULL,                                   "Modifies the nature resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 5,         0,                    1 },
		{ "nature",          '9', NULL,                                   "Modifies the nature resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 5,         0,                    1 },
		{ "frost",           '8', NULL,                                   "Modifies the frost resistance of selected target.",               NULL, UNIT_FIELD_RESISTANCES + 4,         0,                    1 },
		{ "frost",           '9', NULL,                                   "Modifies the frost resistance of selected target.",               NULL, UNIT_FIELD_RESISTANCES + 4,         0,                    1 },
		{ "shadow",          '8', NULL,                                   "Modifies the shadow resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 8,         0,                    1 },
		{ "shadow",          '9', NULL,                                   "Modifies the shadow resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 8,         0,                    1 },
		{ "arcane",          '8', NULL,                                   "Modifies the arcane resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 9,         0,                    1 },
		{ "arcane",          '9', NULL,                                   "Modifies the arcane resistance of selected target.",              NULL, UNIT_FIELD_RESISTANCES + 9,         0,                    1 },
		{ "damage",          '8', NULL,                                   "Modifies the damage done by the selected target.",                NULL, UNIT_FIELD_MINDAMAGE,              UNIT_FIELD_MAXDAMAGE, 2 },
		{ "damage",          '9', NULL,                                   "Modifies the damage done by the selected target.",                NULL, UNIT_FIELD_MINDAMAGE,              UNIT_FIELD_MAXDAMAGE, 2 },
		{ "ap",              '8', NULL,                                   "Modifies the attack power of the selected target.",               NULL, UNIT_FIELD_ATTACK_POWER,           0,                    1 },
		{ "ap",              '9', NULL,                                   "Modifies the attack power of the selected target.",               NULL, UNIT_FIELD_ATTACK_POWER,           0,                    1 },
		{ "rangeap",         '8', NULL,                                   "Modifies the range attack power of the selected target.",         NULL, UNIT_FIELD_RANGED_ATTACK_POWER,    0,                    1 },
		{ "rangeap",         '9', NULL,                                   "Modifies the range attack power of the selected target.",         NULL, UNIT_FIELD_RANGED_ATTACK_POWER,    0,                    1 },
		{ "scale",           '8', NULL,                                   "Modifies the scale of the selected target.",                      NULL, OBJECT_FIELD_SCALE_X,              0,                    2 },
		{ "scale",           '9', NULL,                                   "Modifies the scale of the selected target.",                      NULL, OBJECT_FIELD_SCALE_X,              0,                    2 },
		{ "gold",            '2', &ChatHandler::HandleModifyGoldCommand,  "Modifies the gold amount of the selected target. Copper value.",  NULL, 0,                                 0,                    0 },
		{ "gold",            '5', &ChatHandler::HandleModifyGoldCommand,  "Modifies the gold amount of the selected target. Copper value.",  NULL, 0,                                 0,                    0 },
		{ "gold",            '8', &ChatHandler::HandleModifyGoldCommand,  "Modifies the gold amount of the selected target. Copper value.",  NULL, 0,                                 0,                    0 },
		{ "gold",            '9', &ChatHandler::HandleModifyGoldCommand,  "Modifies the gold amount of the selected target. Copper value.",  NULL, 0,                                 0,                    0 },
		{ "speed",           '2', &ChatHandler::HandleModifySpeedCommand, "Modifies the movement speed of the selected target.",             NULL, 0,                                 0,                    0 },
		{ "speed",           '5', &ChatHandler::HandleModifySpeedCommand, "Modifies the movement speed of the selected target.",             NULL, 0,                                 0,                    0 },
		{ "speed",           '8', &ChatHandler::HandleModifySpeedCommand, "Modifies the movement speed of the selected target.",             NULL, 0,                                 0,                    0 },
		{ "speed",           '9', &ChatHandler::HandleModifySpeedCommand, "Modifies the movement speed of the selected target.",             NULL, 0,                                 0,                    0 },
		{ "nativedisplayid", '8', NULL,                                   "Modifies the native display identifier of the target.",           NULL, UNIT_FIELD_NATIVEDISPLAYID,        0,                    1 },
		{ "nativedisplayid", '9', NULL,                                   "Modifies the native display identifier of the target.",           NULL, UNIT_FIELD_NATIVEDISPLAYID,        0,                    1 },
		{ "displayid",       '2', NULL,                                   "Modifies the display identifier (DisplayID) of the target.",      NULL, UNIT_FIELD_DISPLAYID,              0,                    1 },
		{ "displayid",       '5', NULL,                                   "Modifies the display identifier (DisplayID) of the target.",      NULL, UNIT_FIELD_DISPLAYID,              0,                    1 },
		{ "displayid",       '8', NULL,                                   "Modifies the display identifier (DisplayID) of the target.",      NULL, UNIT_FIELD_DISPLAYID,              0,                    1 },
		{ "displayid",       '9', NULL,                                   "Modifies the display identifier (DisplayID) of the target.",      NULL, UNIT_FIELD_DISPLAYID,              0,                    1 },
		{ "flags",           '8', NULL,                                   "Modifies the flags of the selected target.",                      NULL, UNIT_FIELD_FLAGS,                  0,                    1 },
		{ "flags",           '9', NULL,                                   "Modifies the flags of the selected target.",                      NULL, UNIT_FIELD_FLAGS,                  0,                    1 },
		{ "faction",         '8', NULL,                                   "Modifies the faction template of the selected target.",           NULL, UNIT_FIELD_FACTIONTEMPLATE,        0,                    1 },
		{ "faction",         '9', NULL,                                   "Modifies the faction template of the selected target.",           NULL, UNIT_FIELD_FACTIONTEMPLATE,        0,                    1 },
		{ "dynamicflags",    '8', NULL,                                   "Modifies the dynamic flags of the selected target.",              NULL, UNIT_DYNAMIC_FLAGS,                0,                    1 },
		{ "dynamicflags",    '9', NULL,                                   "Modifies the dynamic flags of the selected target.",              NULL, UNIT_DYNAMIC_FLAGS,                0,                    1 },
		{ "talentpoints",    '9', &ChatHandler::HandleModifyTPsCommand,	  "Modifies the available talent points of the selected target.",    NULL, 0,								  0,                    0 },
		{ "happiness",       '8', NULL,                                   "Modifies the happiness value of the selected target.",            NULL, UNIT_FIELD_POWER5,                 UNIT_FIELD_MAXPOWER5, 1 },
		{ "happiness",       '9', NULL,                                   "Modifies the happiness value of the selected target.",            NULL, UNIT_FIELD_POWER5,                 UNIT_FIELD_MAXPOWER5, 1 },
		{ "boundingraidius", '8', NULL,                                   "Modifies the bounding radius of the selected target.",            NULL, UNIT_FIELD_BOUNDINGRADIUS,         0,                    2 },
		{ "boundingraidius", '9', NULL,                                   "Modifies the bounding radius of the selected target.",            NULL, UNIT_FIELD_BOUNDINGRADIUS,         0,                    2 },
		{ "combatreach",     '8', NULL,                                   "Modifies the combat reach of the selected target.",               NULL, UNIT_FIELD_COMBATREACH,            0,                    2 },
		{ "combatreach",     '9', NULL,                                   "Modifies the combat reach of the selected target.",               NULL, UNIT_FIELD_COMBATREACH,            0,                    2 },
		{ "npcemotestate",   '8', NULL,                                   "Modifies the NPC emote state of the selected target.",            NULL, UNIT_NPC_EMOTESTATE,               0,                    1 },
		{ "npcemotestate",   '9', NULL,                                   "Modifies the NPC emote state of the selected target.",            NULL, UNIT_NPC_EMOTESTATE,               0,                    1 },
		{ "bytes0",          '8', NULL,                                   "WARNING! Modifies the bytes0 entry of selected target.",          NULL, UNIT_FIELD_BYTES_0,                0,                    1 },
		{ "bytes0",          '9', NULL,                                   "WARNING! Modifies the bytes0 entry of selected target.",          NULL, UNIT_FIELD_BYTES_0,                0,                    1 },
		{ "bytes1",          '8', NULL,                                   "WARNING! Modifies the bytes1 entry of selected target.",          NULL, UNIT_FIELD_BYTES_1,                0,                    1 },
		{ "bytes1",          '9', NULL,                                   "WARNING! Modifies the bytes1 entry of selected target.",          NULL, UNIT_FIELD_BYTES_1,                0,                    1 },
		{ "bytes2",          '8', NULL,                                   "WARNING! Modifies the bytes2 entry of selected target.",          NULL, UNIT_FIELD_BYTES_2,                0,                    1 },
		{ "bytes2",          '9', NULL,                                   "WARNING! Modifies the bytes2 entry of selected target.",          NULL, UNIT_FIELD_BYTES_2,                0,                    1 },
		{ NULL,              '0', NULL,                                   "",                                                                NULL, 0,                                 0,                    0 }
	};
	dupe_command_table(modifyCommandTable, _modifyCommandTable);

	static ChatCommand debugCommandTable[] =
	{
		{ "infront",             '8', &ChatHandler::HandleDebugInFrontCommand,     "",                                                                                                                  NULL, 0, 0, 0 },
		{ "infront",             '9', &ChatHandler::HandleDebugInFrontCommand,     "",                                                                                                                  NULL, 0, 0, 0 },
		{ "showreact",           '8', &ChatHandler::HandleShowReactionCommand,     "",                                                                                                                  NULL, 0, 0, 0 },
		{ "showreact",           '9', &ChatHandler::HandleShowReactionCommand,     "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aimove",              '8', &ChatHandler::HandleAIMoveCommand,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aimove",              '9', &ChatHandler::HandleAIMoveCommand,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "dist",                '8', &ChatHandler::HandleDistanceCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "dist",                '9', &ChatHandler::HandleDistanceCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "face",                '8', &ChatHandler::HandleFaceCommand,             "",                                                                                                                  NULL, 0, 0, 0 },
		{ "face",                '9', &ChatHandler::HandleFaceCommand,             "",                                                                                                                  NULL, 0, 0, 0 },
		{ "moveinfo",            '8', &ChatHandler::HandleMoveInfoCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "moveinfo",            '9', &ChatHandler::HandleMoveInfoCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "setbytes",            '8', &ChatHandler::HandleSetBytesCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "setbytes",            '9', &ChatHandler::HandleSetBytesCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "getbytes",            '8', &ChatHandler::HandleGetBytesCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "getbytes",            '9', &ChatHandler::HandleGetBytesCommand,         "",                                                                                                                  NULL, 0, 0, 0 },
		{ "unroot",              '8', &ChatHandler::HandleDebugUnroot,             "",                                                                                                                  NULL, 0, 0, 0 },
		{ "unroot",              '9', &ChatHandler::HandleDebugUnroot,             "",                                                                                                                  NULL, 0, 0, 0 },
		{ "root",                '8', &ChatHandler::HandleDebugRoot,               "",                                                                                                                  NULL, 0, 0, 0 },
		{ "root",                '9', &ChatHandler::HandleDebugRoot,               "",                                                                                                                  NULL, 0, 0, 0 },
		{ "landwalk",            '8', &ChatHandler::HandleDebugLandWalk,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "landwalk",            '9', &ChatHandler::HandleDebugLandWalk,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "waterwalk",           '8', &ChatHandler::HandleDebugWaterWalk,          "",                                                                                                                  NULL, 0, 0, 0 },
		{ "waterwalk",           '9', &ChatHandler::HandleDebugWaterWalk,          "",                                                                                                                  NULL, 0, 0, 0 },
		{ "castspell",           '8', &ChatHandler::HandleCastSpellCommand,        ".castspell <spellid> - Casts spell on target.",                                                                     NULL, 0, 0, 0 },
		{ "castspell",           '9', &ChatHandler::HandleCastSpellCommand,        ".castspell <spellid> - Casts spell on target.",                                                                     NULL, 0, 0, 0 },
		{ "castself",            '8', &ChatHandler::HandleCastSelfCommand,         ".castself <spellId> - Target casts spell <spellId> on itself.",                                                     NULL, 0, 0, 0 },
		{ "castself",            '9', &ChatHandler::HandleCastSelfCommand,         ".castself <spellId> - Target casts spell <spellId> on itself.",                                                     NULL, 0, 0, 0 },
		{ "castspellne",         '8', &ChatHandler::HandleCastSpellNECommand,      ".castspellne <spellid> - Casts spell on target (only plays animations, doesn't handle effects or range/facing/etc.", NULL, 0, 0, 0 },
		{ "castspellne",         '9', &ChatHandler::HandleCastSpellNECommand,      ".castspellne <spellid> - Casts spell on target (only plays animations, doesn't handle effects or range/facing/etc.", NULL, 0, 0, 0 },
		{ "aggrorange",          '8', &ChatHandler::HandleAggroRangeCommand,       ".aggrorange - Shows aggro Range of the selected Creature.",                                                         NULL, 0, 0, 0 },
		{ "aggrorange",          '9', &ChatHandler::HandleAggroRangeCommand,       ".aggrorange - Shows aggro Range of the selected Creature.",                                                         NULL, 0, 0, 0 },
		{ "knockback",           '8', &ChatHandler::HandleKnockBackCommand,        ".knockback <value> - Knocks you back.",                                                                             NULL, 0, 0, 0 },
		{ "knockback",           '9', &ChatHandler::HandleKnockBackCommand,        ".knockback <value> - Knocks you back.",                                                                             NULL, 0, 0, 0 },
		{ "fade",                '8', &ChatHandler::HandleFadeCommand,             ".fade <value> - calls ModThreatModifyer().",                                                                        NULL, 0, 0, 0 },
		{ "fade",                '9', &ChatHandler::HandleFadeCommand,             ".fade <value> - calls ModThreatModifyer().",                                                                        NULL, 0, 0, 0 },
		{ "threatMod",           '8', &ChatHandler::HandleThreatModCommand,        ".threatMod <value> - calls ModGeneratedThreatModifyer().",                                                          NULL, 0, 0, 0 },
		{ "threatMod",           '9', &ChatHandler::HandleThreatModCommand,        ".threatMod <value> - calls ModGeneratedThreatModifyer().",                                                          NULL, 0, 0, 0 },
		{ "calcThreat",          '8', &ChatHandler::HandleCalcThreatCommand,       ".calcThreat <dmg> <spellId> - calculates threat.",                                                                  NULL, 0, 0, 0 },
		{ "calcThreat",          '9', &ChatHandler::HandleCalcThreatCommand,       ".calcThreat <dmg> <spellId> - calculates threat.",                                                                  NULL, 0, 0, 0 },
		{ "threatList",          '8', &ChatHandler::HandleThreatListCommand,       ".threatList  - returns all AI_Targets of the selected Creature.",                                                   NULL, 0, 0, 0 },
		{ "threatList",          '9', &ChatHandler::HandleThreatListCommand,       ".threatList  - returns all AI_Targets of the selected Creature.",                                                   NULL, 0, 0, 0 },
		{ "gettptime",           '8', &ChatHandler::HandleGetTransporterTime,      "grabs transporter travel time",                                                                                     NULL, 0, 0, 0 },
		{ "gettptime",           '9', &ChatHandler::HandleGetTransporterTime,      "grabs transporter travel time",                                                                                     NULL, 0, 0, 0 },
		{ "itempushresult",      '8', &ChatHandler::HandleSendItemPushResult,      "sends item push result",                                                                                            NULL, 0, 0, 0 },
		{ "itempushresult",      '9', &ChatHandler::HandleSendItemPushResult,      "sends item push result",                                                                                            NULL, 0, 0, 0 },
		{ "setbit",              '8', &ChatHandler::HandleModifyBitCommand,        "",                                                                                                                  NULL, 0, 0, 0 },
		{ "setbit",              '9', &ChatHandler::HandleModifyBitCommand,        "",                                                                                                                  NULL, 0, 0, 0 },
		{ "setvalue",            '8', &ChatHandler::HandleModifyValueCommand,      "",                                                                                                                  NULL, 0, 0, 0 },
		{ "setvalue",            '9', &ChatHandler::HandleModifyValueCommand,      "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestbegin",    '8', &ChatHandler::HandleAIAgentDebugBegin,       "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestbegin",    '9', &ChatHandler::HandleAIAgentDebugBegin,       "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestcontinue", '8', &ChatHandler::HandleAIAgentDebugContinue,    "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestcontinue", '9', &ChatHandler::HandleAIAgentDebugContinue,    "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestskip",     '8', &ChatHandler::HandleAIAgentDebugSkip,        "",                                                                                                                  NULL, 0, 0, 0 },
		{ "aispelltestskip",     '9', &ChatHandler::HandleAIAgentDebugSkip,        "",                                                                                                                  NULL, 0, 0, 0 },
		{ "dumpcoords",          '8', &ChatHandler::HandleDebugDumpCoordsCommmand, "",                                                                                                                  NULL, 0, 0, 0 },
		{ "dumpcoords",          '9', &ChatHandler::HandleDebugDumpCoordsCommmand, "",                                                                                                                  NULL, 0, 0, 0 },
		{ "sendpacket",          '8', &ChatHandler::HandleSendpacket,              "<opcode ID>, <data>",                                                                                               NULL, 0, 0, 0 },
		{ "sendpacket",          '9', &ChatHandler::HandleSendpacket,              "<opcode ID>, <data>",                                                                                               NULL, 0, 0, 0 },
		{ "sqlquery",            '8', &ChatHandler::HandleSQLQueryCommand,         "<sql query>",                                                                                                       NULL, 0, 0, 0 },
		{ "sqlquery",            '9', &ChatHandler::HandleSQLQueryCommand,         "<sql query>",                                                                                                       NULL, 0, 0, 0 },
		{ "rangecheck",          '8', &ChatHandler::HandleRangeCheckCommand,       "Checks the 'yard' range and internal range between the player and the target.",                                     NULL, 0, 0, 0 },
		{ "rangecheck",          '9', &ChatHandler::HandleRangeCheckCommand,       "Checks the 'yard' range and internal range between the player and the target.",                                     NULL, 0, 0, 0 },
		{ "setallratings",       '8', &ChatHandler::HandleRatingsCommand,          "Sets rating values to incremental numbers based on their index.",                                                   NULL, 0, 0, 0 },
		{ "setallratings",       '9', &ChatHandler::HandleRatingsCommand,          "Sets rating values to incremental numbers based on their index.",                                                   NULL, 0, 0, 0 },
		{ "testlos",             '8', &ChatHandler::HandleCollisionTestLOS,        "tests los",                                                                                                         NULL, 0, 0, 0 },
		{ "testlos",             '9', &ChatHandler::HandleCollisionTestLOS,        "tests los",                                                                                                         NULL, 0, 0, 0 },
		{ "testindoor",          '8', &ChatHandler::HandleCollisionTestIndoor,     "tests indoor",                                                                                                      NULL, 0, 0, 0 },
		{ "testindoor",          '9', &ChatHandler::HandleCollisionTestIndoor,     "tests indoor",                                                                                                      NULL, 0, 0, 0 },
		{ "getheight",           '8', &ChatHandler::HandleCollisionGetHeight,      "Gets height",                                                                                                       NULL, 0, 0, 0 },
		{ "getheight",           '9', &ChatHandler::HandleCollisionGetHeight,      "Gets height",                                                                                                       NULL, 0, 0, 0 },
		{ "deathstate",          '8', &ChatHandler::HandleGetDeathState,           "returns current deathstate for target",                                                                             NULL, 0, 0, 0 },
		{ "deathstate",          '9', &ChatHandler::HandleGetDeathState,           "returns current deathstate for target",                                                                             NULL, 0, 0, 0 },
		{ "getpos",              '8', &ChatHandler::HandleGetPosCommand,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "getpos",              '9', &ChatHandler::HandleGetPosCommand,           "",                                                                                                                  NULL, 0, 0, 0 },
		{ "sendfailed",			 '8', &ChatHandler::HandleSendFailed,      "",                                                                                                                  NULL, 0, 0, 0 },
		{ "sendfailed",			 '9', &ChatHandler::HandleSendFailed,      "",                                                                                                                  NULL, 0, 0, 0 },
		{ "playmovie",			 '8', &ChatHandler::HandlePlayMovie,			   "Triggers a movie for a player",									NULL, 0, 0, 0 },
		{ "playmovie",			 '9', &ChatHandler::HandlePlayMovie,			   "Triggers a movie for a player",									NULL, 0, 0, 0 },
		{ "auraupdate",			 '8', &ChatHandler::HandleAuraUpdateAdd,			   "<SpellID> <Flags> <StackCount> (caster guid = player target)",									NULL, 0, 0, 0 },
		{ "auraupdate",			 '9', &ChatHandler::HandleAuraUpdateAdd,			   "<SpellID> <Flags> <StackCount> (caster guid = player target)",									NULL, 0, 0, 0 },
		{ "auraremove",			 '8', &ChatHandler::HandleAuraUpdateRemove,			   "<VisualSlot>",									NULL, 0, 0, 0 },
		{ "auraremove",			 '9', &ChatHandler::HandleAuraUpdateRemove,			   "<VisualSlot>",									NULL, 0, 0, 0 },
		{ "spawnwar",			 '8', &ChatHandler::HandleDebugSpawnWarCommand,	   "Spawns desired amount of npcs to fight with eachother",																NULL, 0, 0, 0 },
		{ "spawnwar",			 '9', &ChatHandler::HandleDebugSpawnWarCommand,	   "Spawns desired amount of npcs to fight with eachother",																NULL, 0, 0, 0 },
		{ "updateworldstate",    '8', &ChatHandler::HandleUpdateWorldStateCommand, "Sets the specified worldstate field to the specified value",                                                        NULL, 0, 0, 0 },
		{ "updateworldstate",    '9', &ChatHandler::HandleUpdateWorldStateCommand, "Sets the specified worldstate field to the specified value",                                                        NULL, 0, 0, 0 },
		{ "initworldstates",     '8', &ChatHandler::HandleInitWorldStatesCommand,  "(re)initializes the worldstates.",                                                                                  NULL, 0, 0, 0 },
		{ "initworldstates",     '9', &ChatHandler::HandleInitWorldStatesCommand,  "(re)initializes the worldstates.",                                                                                  NULL, 0, 0, 0 },
		{ "clearworldstates",    '8', &ChatHandler::HandleClearWorldStatesCommand, "Clears the worldstates",                                                                                            NULL, 0, 0, 0 },
		{ "clearworldstates",    '9', &ChatHandler::HandleClearWorldStatesCommand, "Clears the worldstates",                                                                                            NULL, 0, 0, 0 },
		{ NULL,                  '0', NULL,                                        "",                                                                                                                  NULL, 0, 0, 0 }
	};
	dupe_command_table(debugCommandTable, _debugCommandTable);

	static ChatCommand waypointCommandTable[] =
	{
		{ "add",       '8', &ChatHandler::HandleWPAddCommand,          "Add wp at current pos",  NULL, 0, 0, 0 },
		{ "add",       '9', &ChatHandler::HandleWPAddCommand,          "Add wp at current pos",  NULL, 0, 0, 0 },
		{ "show",      '8', &ChatHandler::HandleWPShowCommand,         "Show wp's for creature", NULL, 0, 0, 0 },
		{ "show",      '9', &ChatHandler::HandleWPShowCommand,         "Show wp's for creature", NULL, 0, 0, 0 },
		{ "hide",      '8', &ChatHandler::HandleWPHideCommand,         "Hide wp's for creature", NULL, 0, 0, 0 },
		{ "hide",      '9', &ChatHandler::HandleWPHideCommand,         "Hide wp's for creature", NULL, 0, 0, 0 },
		{ "delete",    '8', &ChatHandler::HandleWPDeleteCommand,       "Delete selected wp",     NULL, 0, 0, 0 },
		{ "delete",    '9', &ChatHandler::HandleWPDeleteCommand,       "Delete selected wp",     NULL, 0, 0, 0 },
		{ "movehere",  '8', &ChatHandler::HandleWPMoveHereCommand,     "Move to this wp",        NULL, 0, 0, 0 },
		{ "movehere",  '9', &ChatHandler::HandleWPMoveHereCommand,     "Move to this wp",        NULL, 0, 0, 0 },
		{ "flags",     '8', &ChatHandler::HandleWPFlagsCommand,        "Wp flags",               NULL, 0, 0, 0 },
		{ "flags",     '9', &ChatHandler::HandleWPFlagsCommand,        "Wp flags",               NULL, 0, 0, 0 },
		{ "waittime",  '8', &ChatHandler::HandleWPWaitCommand,         "Wait time at this wp",   NULL, 0, 0, 0 },
		{ "waittime",  '9', &ChatHandler::HandleWPWaitCommand,         "Wait time at this wp",   NULL, 0, 0, 0 },
		{ "emote",     '8', &ChatHandler::HandleWPEmoteCommand,        "Emote at this wp",       NULL, 0, 0, 0 },
		{ "emote",     '9', &ChatHandler::HandleWPEmoteCommand,        "Emote at this wp",       NULL, 0, 0, 0 },
		{ "skin",      '8', &ChatHandler::HandleWPSkinCommand,         "Skin at this wp",        NULL, 0, 0, 0 },
		{ "skin",      '9', &ChatHandler::HandleWPSkinCommand,         "Skin at this wp",        NULL, 0, 0, 0 },
		{ "change",    '8', &ChatHandler::HandleWPChangeNoCommand,     "Change at this wp",      NULL, 0, 0, 0 },
		{ "change",    '9', &ChatHandler::HandleWPChangeNoCommand,     "Change at this wp",      NULL, 0, 0, 0 },
		{ "info",      '8', &ChatHandler::HandleWPInfoCommand,         "Show info for wp",       NULL, 0, 0, 0 },
		{ "info",      '9', &ChatHandler::HandleWPInfoCommand,         "Show info for wp",       NULL, 0, 0, 0 },
		{ "movetype",  '8', &ChatHandler::HandleWPMoveTypeCommand,     "Movement type at wp",    NULL, 0, 0, 0 },
		{ "movetype",  '9', &ChatHandler::HandleWPMoveTypeCommand,     "Movement type at wp",    NULL, 0, 0, 0 },
		{ "generate",  '8', &ChatHandler::HandleGenerateWaypoints,     "Randomly generate wps",  NULL, 0, 0, 0 },
		{ "generate",  '9', &ChatHandler::HandleGenerateWaypoints,     "Randomly generate wps",  NULL, 0, 0, 0 },
		{ "save",      '8', &ChatHandler::HandleSaveWaypoints,         "Save all waypoints",     NULL, 0, 0, 0 },
		{ "save",      '9', &ChatHandler::HandleSaveWaypoints,         "Save all waypoints",     NULL, 0, 0, 0 },
		{ "deleteall", '8', &ChatHandler::HandleDeleteWaypoints,       "Delete all waypoints",   NULL, 0, 0, 0 },
		{ "deleteall", '9', &ChatHandler::HandleDeleteWaypoints,       "Delete all waypoints",   NULL, 0, 0, 0 },
		{ "addfly",    '8', &ChatHandler::HandleWaypointAddFlyCommand, "Adds a flying waypoint", NULL, 0, 0, 0 },
		{ "addfly",    '9', &ChatHandler::HandleWaypointAddFlyCommand, "Adds a flying waypoint", NULL, 0, 0, 0 },
		{ NULL,        '0', NULL,                                      "",                       NULL, 0, 0, 0 }
	};
	dupe_command_table(waypointCommandTable, _waypointCommandTable);

	static ChatCommand GMTicketCommandTable[] =
	{
#ifdef GM_TICKET_MY_MASTER_COMPATIBLE
		{ "get",             '2', &ChatHandler::HandleGMTicketListCommand,                     "Gets 2 Ticket list.",                                          NULL, 0, 0, 0 },
		{ "get",             '5', &ChatHandler::HandleGMTicketListCommand,                     "Gets 2 Ticket list.",                                          NULL, 0, 0, 0 },
		{ "get",             '8', &ChatHandler::HandleGMTicketListCommand,                     "Gets 2 Ticket list.",                                          NULL, 0, 0, 0 },
		{ "get",             '9', &ChatHandler::HandleGMTicketListCommand,                     "Gets 2 Ticket list.",                                          NULL, 0, 0, 0 },
		{ "getId",           '2', &ChatHandler::HandleGMTicketGetByIdCommand,                  "Gets 2 Ticket by player name.",                                NULL, 0, 0, 0 },
		{ "getId",           '5', &ChatHandler::HandleGMTicketGetByIdCommand,                  "Gets 2 Ticket by player name.",                                NULL, 0, 0, 0 },
		{ "getId",           '8', &ChatHandler::HandleGMTicketGetByIdCommand,                  "Gets 2 Ticket by player name.",                                NULL, 0, 0, 0 },
		{ "getId",           '9', &ChatHandler::HandleGMTicketGetByIdCommand,                  "Gets 2 Ticket by player name.",                                NULL, 0, 0, 0 },
		{ "delId",           '2', &ChatHandler::HandleGMTicketRemoveByIdCommand,               "Deletes 2 Ticket by player name.",                             NULL, 0, 0, 0 },
		{ "delId",           '5', &ChatHandler::HandleGMTicketRemoveByIdCommand,               "Deletes 2 Ticket by player name.",                             NULL, 0, 0, 0 },
		{ "delId",           '8', &ChatHandler::HandleGMTicketRemoveByIdCommand,               "Deletes 2 Ticket by player name.",                             NULL, 0, 0, 0 },
		{ "delId",           '9', &ChatHandler::HandleGMTicketRemoveByIdCommand,               "Deletes 2 Ticket by player name.",                             NULL, 0, 0, 0 },
#else
		{ "list",            'c', &ChatHandler::HandleGMTicketListCommand,                     "Lists all active 2 Tickets.",                                  NULL, 0, 0, 0 },
		{ "get",             'c', &ChatHandler::HandleGMTicketGetByIdCommand,                  "Gets 2 Ticket with ID x.",                                     NULL, 0, 0, 0 },
		{ "remove",          'c', &ChatHandler::HandleGMTicketRemoveByIdCommand,               "Removes 2 Ticket with ID x.",                                  NULL, 0, 0, 0 },
		{ "deletepermanent", 'z', &ChatHandler::HandleGMTicketDeletePermanentCommand,          "Deletes 2 Ticket with ID x permanently.",                      NULL, 0, 0, 0 },
		{ "assign",          'c', &ChatHandler::HandleGMTicketAssignToCommand,                 "Assigns 2 Ticket with id x to 2 y (if empty to your self).", NULL, 0, 0, 0 },
		{ "release",         'c', &ChatHandler::HandleGMTicketReleaseCommand,                  "Releases assigned 2 Ticket with ID x.",                        NULL, 0, 0, 0 },
		{ "comment",         'c', &ChatHandler::HandleGMTicketCommentCommand,                  "Sets comment x to 2 Ticket with ID y.",                        NULL, 0, 0, 0 },
#endif
		{ "toggle",          '8', &ChatHandler::HandleGMTicketToggleTicketSystemStatusCommand, "Toggles the ticket system status.",                             NULL, 0, 0, 0 },
		{ "toggle",          '9', &ChatHandler::HandleGMTicketToggleTicketSystemStatusCommand, "Toggles the ticket system status.",                             NULL, 0, 0, 0 },
		{ NULL,              '0', NULL,                                                        "",                                                              NULL, 0, 0, 0 }
	};
	dupe_command_table(GMTicketCommandTable, _GMTicketCommandTable);

	static ChatCommand GuildCommandTable[] =
	{
		{ "join",         '2', &ChatHandler::HandleGuildJoinCommand,         "Force joins a guild",                 NULL, 0, 0, 0 },
		{ "join",         '5', &ChatHandler::HandleGuildJoinCommand,         "Force joins a guild",                 NULL, 0, 0, 0 },
		{ "join",         '8', &ChatHandler::HandleGuildJoinCommand,         "Force joins a guild",                 NULL, 0, 0, 0 },
		{ "join",         '9', &ChatHandler::HandleGuildJoinCommand,         "Force joins a guild",                 NULL, 0, 0, 0 },
		{ "create",       '0', &ChatHandler::CreateGuildCommand,             "Creates a guild.",                    NULL, 0, 0, 0 },
		{ "rename",       '5', &ChatHandler::HandleRenameGuildCommand,       "Renames a guild.",                    NULL, 0, 0, 0 },
		{ "rename",       '8', &ChatHandler::HandleRenameGuildCommand,       "Renames a guild.",                    NULL, 0, 0, 0 },
		{ "rename",       '9', &ChatHandler::HandleRenameGuildCommand,       "Renames a guild.",                    NULL, 0, 0, 0 },
		{ "members",      '2', &ChatHandler::HandleGuildMembersCommand,      "Lists guildmembers and their ranks.", NULL, 0, 0, 0 },
		{ "members",      '5', &ChatHandler::HandleGuildMembersCommand,      "Lists guildmembers and their ranks.", NULL, 0, 0, 0 },
		{ "members",      '8', &ChatHandler::HandleGuildMembersCommand,      "Lists guildmembers and their ranks.", NULL, 0, 0, 0 },
		{ "members",      '9', &ChatHandler::HandleGuildMembersCommand,      "Lists guildmembers and their ranks.", NULL, 0, 0, 0 },
		{ "removeplayer", '2', &ChatHandler::HandleGuildRemovePlayerCommand, "Removes a player from a guild.",      NULL, 0, 0, 0 },
		{ "removeplayer", '5', &ChatHandler::HandleGuildRemovePlayerCommand, "Removes a player from a guild.",      NULL, 0, 0, 0 },
		{ "removeplayer", '8', &ChatHandler::HandleGuildRemovePlayerCommand, "Removes a player from a guild.",      NULL, 0, 0, 0 },
		{ "removeplayer", '9', &ChatHandler::HandleGuildRemovePlayerCommand, "Removes a player from a guild.",      NULL, 0, 0, 0 },
		{ "disband",      '5', &ChatHandler::HandleGuildDisbandCommand,      "Disbands the guild of your target.",  NULL, 0, 0, 0 },
		{ "disband",      '8', &ChatHandler::HandleGuildDisbandCommand,      "Disbands the guild of your target.",  NULL, 0, 0, 0 },
		{ "disband",      '9', &ChatHandler::HandleGuildDisbandCommand,      "Disbands the guild of your target.",  NULL, 0, 0, 0 },
		{ NULL,           '0', NULL,                                         "",                                    NULL, 0, 0, 0 }
	};
	dupe_command_table(GuildCommandTable, _GuildCommandTable);

	static ChatCommand GameObjectCommandTable[] =
	{
		{ "select",       '2', &ChatHandler::HandleGOSelect,       "Selects the nearest GameObject to you",    NULL, 0, 0, 0 },
		{ "select",       '5', &ChatHandler::HandleGOSelect,       "Selects the nearest GameObject to you",    NULL, 0, 0, 0 },
		{ "select",       '8', &ChatHandler::HandleGOSelect,       "Selects the nearest GameObject to you",    NULL, 0, 0, 0 },
		{ "select",       '9', &ChatHandler::HandleGOSelect,       "Selects the nearest GameObject to you",    NULL, 0, 0, 0 },
		{ "delete",       '2', &ChatHandler::HandleGODelete,       "Deletes selected GameObject",              NULL, 0, 0, 0 },
		{ "delete",       '5', &ChatHandler::HandleGODelete,       "Deletes selected GameObject",              NULL, 0, 0, 0 },
		{ "delete",       '8', &ChatHandler::HandleGODelete,       "Deletes selected GameObject",              NULL, 0, 0, 0 },
		{ "delete",       '9', &ChatHandler::HandleGODelete,       "Deletes selected GameObject",              NULL, 0, 0, 0 },
		{ "spawn",        '2', &ChatHandler::HandleGOSpawn,        "Spawns a GameObject by ID",                NULL, 0, 0, 0 },
		{ "spawn",        '5', &ChatHandler::HandleGOSpawn,        "Spawns a GameObject by ID",                NULL, 0, 0, 0 },
		{ "spawn",        '8', &ChatHandler::HandleGOSpawn,        "Spawns a GameObject by ID",                NULL, 0, 0, 0 },
		{ "spawn",        '9', &ChatHandler::HandleGOSpawn,        "Spawns a GameObject by ID",                NULL, 0, 0, 0 },
		{ "phase",        '8', &ChatHandler::HandleGOPhaseCommand, "<phase> <save> - Phase selected GameObject", NULL, 0, 0, 0 },
		{ "phase",        '9', &ChatHandler::HandleGOPhaseCommand, "<phase> <save> - Phase selected GameObject", NULL, 0, 0, 0 },
		{ "info",         '2', &ChatHandler::HandleGOInfo,         "Gives you information about selected GO", NULL, 0, 0, 0 },
		{ "info",         '5', &ChatHandler::HandleGOInfo,         "Gives you information about selected GO", NULL, 0, 0, 0 },
		{ "info",         '8', &ChatHandler::HandleGOInfo,         "Gives you information about selected GO", NULL, 0, 0, 0 },
		{ "info",         '9', &ChatHandler::HandleGOInfo,         "Gives you information about selected GO", NULL, 0, 0, 0 },
		{ "damage",       '8', &ChatHandler::HandleGODamageCommand,"Damages the GO for the specified hitpoints",NULL, 0, 0, 0},
		{ "damage",       '9', &ChatHandler::HandleGODamageCommand,"Damages the GO for the specified hitpoints",NULL, 0, 0, 0},
		{ "rebuild",      '8', &ChatHandler::HandleGORebuildCommand,"Rebuilds the GO.",                        NULL, 0, 0, 0 },
		{ "rebuild",      '9', &ChatHandler::HandleGORebuildCommand,"Rebuilds the GO.",                        NULL, 0, 0, 0 },
		{ "activate",     '2', &ChatHandler::HandleGOActivate,     "Activates/Opens the selected GO.",         NULL, 0, 0, 0 },
		{ "activate",     '5', &ChatHandler::HandleGOActivate,     "Activates/Opens the selected GO.",         NULL, 0, 0, 0 },
		{ "activate",     '8', &ChatHandler::HandleGOActivate,     "Activates/Opens the selected GO.",         NULL, 0, 0, 0 },
		{ "activate",     '9', &ChatHandler::HandleGOActivate,     "Activates/Opens the selected GO.",         NULL, 0, 0, 0 },
		{ "enable",       '2', &ChatHandler::HandleGOEnable,       "Enables the selected GO for use.",         NULL, 0, 0, 0 },
		{ "enable",       '5', &ChatHandler::HandleGOEnable,       "Enables the selected GO for use.",         NULL, 0, 0, 0 },
		{ "enable",       '8', &ChatHandler::HandleGOEnable,       "Enables the selected GO for use.",         NULL, 0, 0, 0 },
		{ "enable",       '9', &ChatHandler::HandleGOEnable,       "Enables the selected GO for use.",         NULL, 0, 0, 0 },
		{ "scale",        '5', &ChatHandler::HandleGOScale,        "Sets scale of selected GO",                NULL, 0, 0, 0 },
		{ "scale",        '8', &ChatHandler::HandleGOScale,        "Sets scale of selected GO",                NULL, 0, 0, 0 },
		{ "scale",        '9', &ChatHandler::HandleGOScale,        "Sets scale of selected GO",                NULL, 0, 0, 0 },
		{ "animprogress", '8', &ChatHandler::HandleGOAnimProgress, "Sets anim progress",                       NULL, 0, 0, 0 },
		{ "animprogress", '9', &ChatHandler::HandleGOAnimProgress, "Sets anim progress",                       NULL, 0, 0, 0 },
		{ "faction",      '8', &ChatHandler::HandleGOFactionCommand,"Sets the faction of the GO",              NULL, 0, 0, 0 },
		{ "faction",      '9', &ChatHandler::HandleGOFactionCommand,"Sets the faction of the GO",              NULL, 0, 0, 0 },
		{ "export",       '8', &ChatHandler::HandleGOExport,       "Exports the current GO selected",          NULL, 0, 0, 0 },
		{ "export",       '9', &ChatHandler::HandleGOExport,       "Exports the current GO selected",          NULL, 0, 0, 0 },
		{ "move",         '5', &ChatHandler::HandleGOMove,         "Moves gameobject to player xyz",           NULL, 0, 0, 0 },
		{ "move",         '8', &ChatHandler::HandleGOMove,         "Moves gameobject to player xyz",           NULL, 0, 0, 0 },
		{ "move",         '9', &ChatHandler::HandleGOMove,         "Moves gameobject to player xyz",           NULL, 0, 0, 0 },
		{ "rotate",       '5', &ChatHandler::HandleGORotate,       "<Axis> <Value> - Rotates the object. <Axis> x,y, Default o.",             NULL, 0, 0, 0 },
		{ "rotate",       '8', &ChatHandler::HandleGORotate,       "<Axis> <Value> - Rotates the object. <Axis> x,y, Default o.",             NULL, 0, 0, 0 },
		{ "rotate",       '9', &ChatHandler::HandleGORotate,       "<Axis> <Value> - Rotates the object. <Axis> x,y, Default o.",             NULL, 0, 0, 0 },
		{ "portto",       '8', &ChatHandler::HandlePortToGameObjectSpawnCommand, "Teleports you to the gameobject with spawn id x.", NULL, 0, 0, 0 },
		{ "portto",       '9', &ChatHandler::HandlePortToGameObjectSpawnCommand, "Teleports you to the gameobject with spawn id x.", NULL, 0, 0, 0 },
		{ NULL,           '0', NULL,                               "",                                         NULL, 0, 0, 0 }
	};
	dupe_command_table(GameObjectCommandTable, _GameObjectCommandTable);

	static ChatCommand BattlegroundCommandTable[] =
	{
		{ "setbgscore",    '8', &ChatHandler::HandleSetBGScoreCommand,                       "<Teamid> <Score> - Sets battleground score. 2 Arguments.",      NULL, 0, 0, 0 },
		{ "setbgscore",    '9', &ChatHandler::HandleSetBGScoreCommand,                       "<Teamid> <Score> - Sets battleground score. 2 Arguments.",      NULL, 0, 0, 0 },
		{ "startbg",       '5', &ChatHandler::HandleStartBGCommand,                          "Starts current battleground match.",                            NULL, 0, 0, 0 },
		{ "startbg",       '8', &ChatHandler::HandleStartBGCommand,                          "Starts current battleground match.",                            NULL, 0, 0, 0 },
		{ "startbg",       '9', &ChatHandler::HandleStartBGCommand,                          "Starts current battleground match.",                            NULL, 0, 0, 0 },
		{ "pausebg",       '5', &ChatHandler::HandlePauseBGCommand,                          "Pauses current battleground match.",                            NULL, 0, 0, 0 },
		{ "pausebg",       '8', &ChatHandler::HandlePauseBGCommand,                          "Pauses current battleground match.",                            NULL, 0, 0, 0 },
		{ "pausebg",       '9', &ChatHandler::HandlePauseBGCommand,                          "Pauses current battleground match.",                            NULL, 0, 0, 0 },
		{ "bginfo",        '5', &ChatHandler::HandleBGInfoCommnad,                           "Displays information about current battleground.",              NULL, 0, 0, 0 },
		{ "bginfo",        '8', &ChatHandler::HandleBGInfoCommnad,                           "Displays information about current battleground.",              NULL, 0, 0, 0 },
		{ "bginfo",        '9', &ChatHandler::HandleBGInfoCommnad,                           "Displays information about current battleground.",              NULL, 0, 0, 0 },
		{ "battleground",  '8', &ChatHandler::HandleBattlegroundCommand,                     "Shows BG Menu",                                                 NULL, 0, 0, 0 },
		{ "battleground",  '9', &ChatHandler::HandleBattlegroundCommand,                     "Shows BG Menu",                                                 NULL, 0, 0, 0 },
		{ "setworldstate", '8', &ChatHandler::HandleSetWorldStateCommand,                    "<var> <val> - Var can be in hex. WS Value.",                    NULL, 0, 0, 0 },
		{ "setworldstate", '9', &ChatHandler::HandleSetWorldStateCommand,                    "<var> <val> - Var can be in hex. WS Value.",                    NULL, 0, 0, 0 },
		{ "setworldstates", '8', &ChatHandler::HandleSetWorldStatesCommand,                    "<var> <val> - Var can be in hex. WS Value.",                   NULL, 0, 0, 0 },
		{ "setworldstates", '9', &ChatHandler::HandleSetWorldStatesCommand,                    "<var> <val> - Var can be in hex. WS Value.",                   NULL, 0, 0, 0 },
		{ "playsound",     '8', &ChatHandler::HandlePlaySoundCommand,                        "<val>. Val can be in hex.",                                     NULL, 0, 0, 0 },
		{ "playsound",     '9', &ChatHandler::HandlePlaySoundCommand,                        "<val>. Val can be in hex.",                                     NULL, 0, 0, 0 },
		{ "setbfstatus",   '8', &ChatHandler::HandleSetBattlefieldStatusCommand,             ".setbfstatus - NYI.",                                           NULL, 0, 0, 0 },
		{ "setbfstatus",   '9', &ChatHandler::HandleSetBattlefieldStatusCommand,             ".setbfstatus - NYI.",                                           NULL, 0, 0, 0 },
		{ "leave",         '8', &ChatHandler::HandleBattlegroundExitCommand,                 "Leaves the current battleground.",                              NULL, 0, 0, 0 },
		{ "leave",         '9', &ChatHandler::HandleBattlegroundExitCommand,                 "Leaves the current battleground.",                              NULL, 0, 0, 0 },
		{ "getqueue",      '8', &ChatHandler::HandleGetBattlegroundQueueCommand,             "Gets common battleground queue information.",                   NULL, 0, 0, 0 },
		{ "getqueue",      '9', &ChatHandler::HandleGetBattlegroundQueueCommand,             "Gets common battleground queue information.",                   NULL, 0, 0, 0 },
		{ "forcestart",    '5', &ChatHandler::HandleInitializeAllQueuedBattlegroundsCommand, "Forces initialization of all battlegrounds with active queue.", NULL, 0, 0, 0 },
		{ "forcestart",    '8', &ChatHandler::HandleInitializeAllQueuedBattlegroundsCommand, "Forces initialization of all battlegrounds with active queue.", NULL, 0, 0, 0 },
		{ "forcestart",    '9', &ChatHandler::HandleInitializeAllQueuedBattlegroundsCommand, "Forces initialization of all battlegrounds with active queue.", NULL, 0, 0, 0 },
		{ NULL,            '0', NULL,                                                        "",                                                              NULL, 0, 0, 0 }
	};
	dupe_command_table(BattlegroundCommandTable, _BattlegroundCommandTable);

	static ChatCommand NPCCommandTable[] =
	{
		{ "vendoradditem",    '8', &ChatHandler::HandleItemCommand,           "Adds to vendor",                                                                                                                          NULL, 0, 0, 0 },
		{ "vendoradditem",    '9', &ChatHandler::HandleItemCommand,           "Adds to vendor",                                                                                                                          NULL, 0, 0, 0 },
		{ "vendorremoveitem", '8', &ChatHandler::HandleItemRemoveCommand,     "Removes from vendor.",                                                                                                                    NULL, 0, 0, 0 },
		{ "vendorremoveitem", '9', &ChatHandler::HandleItemRemoveCommand,     "Removes from vendor.",                                                                                                                    NULL, 0, 0, 0 },
		{ "flags",            '8', &ChatHandler::HandleNPCFlagCommand,        "Changes NPC flags",                                                                                                                       NULL, 0, 0, 0 },
		{ "flags",            '9', &ChatHandler::HandleNPCFlagCommand,        "Changes NPC flags",                                                                                                                       NULL, 0, 0, 0 },
		{ "emote",            '8', &ChatHandler::HandleEmoteCommand,          ".emote - Sets emote state",                                                                                                               NULL, 0, 0, 0 },
		{ "emote",            '9', &ChatHandler::HandleEmoteCommand,          ".emote - Sets emote state",                                                                                                               NULL, 0, 0, 0 },
		{ "delete",           '8', &ChatHandler::HandleDeleteCommand,         "Deletes mob from db and world.",                                                                                                          NULL, 0, 0, 0 },
		{ "delete",           '9', &ChatHandler::HandleDeleteCommand,         "Deletes mob from db and world.",                                                                                                          NULL, 0, 0, 0 },
		{ "info",             '8', &ChatHandler::HandleNpcInfoCommand,        "Displays NPC information",                                                                                                                NULL, 0, 0, 0 },
		{ "info",             '9', &ChatHandler::HandleNpcInfoCommand,        "Displays NPC information",                                                                                                                NULL, 0, 0, 0 },
		{ "phase",            '8', &ChatHandler::HandleCreaturePhaseCommand,  "<phase> <save> - Sets phase of selected mob",                                                                                             NULL, 0, 0, 0 },
		{ "phase",            '9', &ChatHandler::HandleCreaturePhaseCommand,  "<phase> <save> - Sets phase of selected mob",                                                                                             NULL, 0, 0, 0 },
		{ "addAgent",         '8', &ChatHandler::HandleAddAIAgentCommand,     ".npc addAgent <agent> <procEvent> <procChance> <procCount> <spellId> <spellType> <spelltargetType> <spellCooldown> <floatMisc1> <Misc2>", NULL, 0, 0, 0 },
		{ "addAgent",         '9', &ChatHandler::HandleAddAIAgentCommand,     ".npc addAgent <agent> <procEvent> <procChance> <procCount> <spellId> <spellType> <spelltargetType> <spellCooldown> <floatMisc1> <Misc2>", NULL, 0, 0, 0 },
		{ "listAgent",        '8', &ChatHandler::HandleListAIAgentCommand,    ".npc listAgent",                                                                                                                          NULL, 0, 0, 0 },
		{ "listAgent",        '9', &ChatHandler::HandleListAIAgentCommand,    ".npc listAgent",                                                                                                                          NULL, 0, 0, 0 },
		{ "say",              '8', &ChatHandler::HandleMonsterSayCommand,     ".npc say <text> - Makes selected mob say text <text>.",                                                                                   NULL, 0, 0, 0 },
		{ "say",              '9', &ChatHandler::HandleMonsterSayCommand,     ".npc say <text> - Makes selected mob say text <text>.",                                                                                   NULL, 0, 0, 0 },
		{ "yell",             '8', &ChatHandler::HandleMonsterYellCommand,    ".npc yell <Text> - Makes selected mob yell text <text>.",                                                                                 NULL, 0, 0, 0 },
		{ "yell",             '9', &ChatHandler::HandleMonsterYellCommand,    ".npc yell <Text> - Makes selected mob yell text <text>.",                                                                                 NULL, 0, 0, 0 },
		{ "come",             '8', &ChatHandler::HandleNpcComeCommand,        ".npc come - Makes npc move to your position",                                                                                             NULL, 0, 0, 0 },
		{ "come",             '9', &ChatHandler::HandleNpcComeCommand,        ".npc come - Makes npc move to your position",                                                                                             NULL, 0, 0, 0 },
		{ "return",           '8', &ChatHandler::HandleNpcReturnCommand,      ".npc return - Returns ncp to spawnpoint.",                                                                                                NULL, 0, 0, 0 },
		{ "return",           '9', &ChatHandler::HandleNpcReturnCommand,      ".npc return - Returns ncp to spawnpoint.",                                                                                                NULL, 0, 0, 0 },
		{ "spawn",            '8', &ChatHandler::HandleCreatureSpawnCommand,  ".npc spawn - Spawns npc of entry <id>",                                                                                                   NULL, 0, 0, 0 },
		{ "spawn",            '9', &ChatHandler::HandleCreatureSpawnCommand,  ".npc spawn - Spawns npc of entry <id>",                                                                                                   NULL, 0, 0, 0 },
		{ "respawn",          '8', &ChatHandler::HandleCreatureRespawnCommand, ".respawn - Respawns a dead npc from its corpse.",                                                                                         NULL, 0, 0, 0 },
		{ "respawn",          '9', &ChatHandler::HandleCreatureRespawnCommand, ".respawn - Respawns a dead npc from its corpse.",                                                                                         NULL, 0, 0, 0 },
		{ "spawnlink",        '8', &ChatHandler::HandleNpcSpawnLinkCommand,   ".spawnlink sqlentry",                                                                                                                     NULL, 0, 0, 0 },
		{ "spawnlink",        '9', &ChatHandler::HandleNpcSpawnLinkCommand,   ".spawnlink sqlentry",                                                                                                                     NULL, 0, 0, 0 },
		{ "possess",          '8', &ChatHandler::HandleNpcPossessCommand,     ".npc possess - Possess an npc (mind control)",                                                                                            NULL, 0, 0, 0 },
		{ "possess",          '9', &ChatHandler::HandleNpcPossessCommand,     ".npc possess - Possess an npc (mind control)",                                                                                            NULL, 0, 0, 0 },
		{ "unpossess",        '8', &ChatHandler::HandleNpcUnPossessCommand,   ".npc unpossess - Unpossess any currently possessed npc.",                                                                                 NULL, 0, 0, 0 },
		{ "unpossess",        '9', &ChatHandler::HandleNpcUnPossessCommand,   ".npc unpossess - Unpossess any currently possessed npc.",                                                                                 NULL, 0, 0, 0 },
		{ "select",           '8', &ChatHandler::HandleNpcSelectCommand,      ".npc select - selects npc closest",                                                                                                       NULL, 0, 0, 0 },
		{ "select",           '9', &ChatHandler::HandleNpcSelectCommand,      ".npc select - selects npc closest",                                                                                                       NULL, 0, 0, 0 },
		{ "npcfollow",        '8', &ChatHandler::HandleNpcFollowCommand,      "Sets npc to follow you",                                                                                                                  NULL, 0, 0, 0 },
		{ "npcfollow",        '9', &ChatHandler::HandleNpcFollowCommand,      "Sets npc to follow you",                                                                                                                  NULL, 0, 0, 0 },
		{ "nullfollow",       '8', &ChatHandler::HandleNullFollowCommand,     "Sets npc to not follow anything",                                                                                                         NULL, 0, 0, 0 },
		{ "nullfollow",       '9', &ChatHandler::HandleNullFollowCommand,     "Sets npc to not follow anything",                                                                                                         NULL, 0, 0, 0 },
		{ "formationlink1",   '8', &ChatHandler::HandleFormationLink1Command, "Sets formation master.",                                                                                                                  NULL, 0, 0, 0 },
		{ "formationlink1",   '9', &ChatHandler::HandleFormationLink1Command, "Sets formation master.",                                                                                                                  NULL, 0, 0, 0 },
		{ "formationlink2",   '8', &ChatHandler::HandleFormationLink2Command, "Sets formation slave with distance and angle",                                                                                            NULL, 0, 0, 0 },
		{ "formationlink2",   '9', &ChatHandler::HandleFormationLink2Command, "Sets formation slave with distance and angle",                                                                                            NULL, 0, 0, 0 },
		{ "formationclear",   '8', &ChatHandler::HandleFormationClearCommand, "Removes formation from creature",                                                                                                         NULL, 0, 0, 0 },
		{ "formationclear",   '9', &ChatHandler::HandleFormationClearCommand, "Removes formation from creature",                                                                                                         NULL, 0, 0, 0 },
		{ "equip1",           '8', &ChatHandler::HandleNPCEquipOneCommand,    "Use: .npc equip1 <itemid> - use .npc equip1 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "equip1",           '9', &ChatHandler::HandleNPCEquipOneCommand,    "Use: .npc equip1 <itemid> - use .npc equip1 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "equip2",           '8', &ChatHandler::HandleNPCEquipTwoCommand,    "Use: .npc equip2 <itemid> - use .npc equip2 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "equip2",           '9', &ChatHandler::HandleNPCEquipTwoCommand,    "Use: .npc equip2 <itemid> - use .npc equip2 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "equip3",           '8', &ChatHandler::HandleNPCEquipThreeCommand,  "Use: .npc equip3 <itemid> - use .npc equip3 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "equip3",           '9', &ChatHandler::HandleNPCEquipThreeCommand,  "Use: .npc equip3 <itemid> - use .npc equip3 0 to remove the item",                                                                        NULL, 0, 0, 0 },
		{ "portto",           '8', &ChatHandler::HandlePortToCreatureSpawnCommand, "Teleports you to the creature with spawn id x.",                                                                                     NULL, 0, 0, 0 },
		{ "portto",           '9', &ChatHandler::HandlePortToCreatureSpawnCommand, "Teleports you to the creature with spawn id x.",                                                                                     NULL, 0, 0, 0 },
		{ "loot",             '8', &ChatHandler::HandleNPCLootCommand,        ".npc loot <quality> - displays possible loot for the selected NPC.",                                                                      NULL, 0, 0, 0 },
		{ "loot",             '9', &ChatHandler::HandleNPCLootCommand,        ".npc loot <quality> - displays possible loot for the selected NPC.",                                                                      NULL, 0, 0, 0 },
		{ "canfly",           '8', &ChatHandler::HandleNPCCanFlyCommand,      ".npc canfly <save> - Toggles CanFly state",                                                                                                      NULL, 0, 0, 0 },
		{ "canfly",           '9', &ChatHandler::HandleNPCCanFlyCommand,      ".npc canfly <save> - Toggles CanFly state",                                                                                                      NULL, 0, 0, 0 },
		{ "ongameobject",     '8', &ChatHandler::HandleNPCOnGOCommand,        ".npc ongameobject <save> - Toggles onGameobject state. Required when spawning a NPC on a Gameobject",                                            NULL, 0, 0, 0 },
		{ "ongameobject",     '9', &ChatHandler::HandleNPCOnGOCommand,        ".npc ongameobject <save> - Toggles onGameobject state. Required when spawning a NPC on a Gameobject",                                            NULL, 0, 0, 0 },
		{ "cast",             '8', &ChatHandler::HandleNPCCastCommand,        ".npc cast < spellid > - Makes the NPC cast this spell.",																							NULL, 0, 0, 0 },
		{ "cast",             '9', &ChatHandler::HandleNPCCastCommand,        ".npc cast < spellid > - Makes the NPC cast this spell.",																							NULL, 0, 0, 0 },
		{ NULL,               '0', NULL,                                      "",                                                                                                                                        NULL, 0, 0, 0 }
	};
	dupe_command_table(NPCCommandTable, _NPCCommandTable);

	static ChatCommand CheatCommandTable[] =
	{
		{ "status",      '2', &ChatHandler::HandleShowCheatsCommand,       "Shows active cheats.",                            NULL, 0, 0, 0 },
		{ "status",      '5', &ChatHandler::HandleShowCheatsCommand,       "Shows active cheats.",                            NULL, 0, 0, 0 },
		{ "status",      '8', &ChatHandler::HandleShowCheatsCommand,       "Shows active cheats.",                            NULL, 0, 0, 0 },
		{ "status",      '9', &ChatHandler::HandleShowCheatsCommand,       "Shows active cheats.",                            NULL, 0, 0, 0 },
		{ "taxi",        '8', &ChatHandler::HandleTaxiCheatCommand,        "Enables all taxi nodes.",                         NULL, 0, 0, 0 },
		{ "taxi",        '9', &ChatHandler::HandleTaxiCheatCommand,        "Enables all taxi nodes.",                         NULL, 0, 0, 0 },
		{ "cooldown",    '8', &ChatHandler::HandleCooldownCheatCommand,    "Enables no cooldown cheat.",                      NULL, 0, 0, 0 },
		{ "cooldown",    '9', &ChatHandler::HandleCooldownCheatCommand,    "Enables no cooldown cheat.",                      NULL, 0, 0, 0 },
		{ "casttime",    '8', &ChatHandler::HandleCastTimeCheatCommand,    "Enables no cast time cheat.",                     NULL, 0, 0, 0 },
		{ "casttime",    '9', &ChatHandler::HandleCastTimeCheatCommand,    "Enables no cast time cheat.",                     NULL, 0, 0, 0 },
		{ "power",       '8', &ChatHandler::HandlePowerCheatCommand,       "Disables mana consumption etc.",                  NULL, 0, 0, 0 },
		{ "power",       '9', &ChatHandler::HandlePowerCheatCommand,       "Disables mana consumption etc.",                  NULL, 0, 0, 0 },
		{ "god",         '8', &ChatHandler::HandleGodModeCommand,          "Sets god mode, prevents you from taking damage.", NULL, 0, 0, 0 },
		{ "god",         '9', &ChatHandler::HandleGodModeCommand,          "Sets god mode, prevents you from taking damage.", NULL, 0, 0, 0 },
		{ "fly",         '2', &ChatHandler::HandleFlyCommand,              "Sets fly mode",                                   NULL, 0, 0, 0 },
		{ "fly",         '5', &ChatHandler::HandleFlyCommand,              "Sets fly mode",                                   NULL, 0, 0, 0 },
		{ "fly",         '8', &ChatHandler::HandleFlyCommand,              "Sets fly mode",                                   NULL, 0, 0, 0 },
		{ "fly",         '9', &ChatHandler::HandleFlyCommand,              "Sets fly mode",                                   NULL, 0, 0, 0 },
		{ "explore",     '8', &ChatHandler::HandleExploreCheatCommand,     "Reveals the unexplored parts of the map.",        NULL, 0, 0, 0 },
		{ "explore",     '9', &ChatHandler::HandleExploreCheatCommand,     "Reveals the unexplored parts of the map.",        NULL, 0, 0, 0 },
		{ "flyspeed",    '2', &ChatHandler::HandleModifySpeedCommand,      "Does the same thing as .modify speed",            NULL, 0, 0, 0 },
		{ "flyspeed",    '5', &ChatHandler::HandleModifySpeedCommand,      "Does the same thing as .modify speed",            NULL, 0, 0, 0 },
		{ "flyspeed",    '8', &ChatHandler::HandleModifySpeedCommand,      "Does the same thing as .modify speed",            NULL, 0, 0, 0 },
		{ "flyspeed",    '9', &ChatHandler::HandleModifySpeedCommand,      "Does the same thing as .modify speed",            NULL, 0, 0, 0 },
		{ "stack",       '8', &ChatHandler::HandleAuraStackCheatCommand,   "Enables aura stacking cheat.",                    NULL, 0, 0, 0 },
		{ "stack",       '9', &ChatHandler::HandleAuraStackCheatCommand,   "Enables aura stacking cheat.",                    NULL, 0, 0, 0 },
		{ "itemstack",   '8', &ChatHandler::HandleItemStackCheatCommand,   "Enables item stacking cheat.",                    NULL, 0, 0, 0 },
		{ "itemstack",   '9', &ChatHandler::HandleItemStackCheatCommand,   "Enables item stacking cheat.",                    NULL, 0, 0, 0 },
		{ "triggerpass", '2', &ChatHandler::HandleTriggerpassCheatCommand, "Ignores area trigger prerequisites.",             NULL, 0, 0, 0 },
		{ "triggerpass", '5', &ChatHandler::HandleTriggerpassCheatCommand, "Ignores area trigger prerequisites.",             NULL, 0, 0, 0 },
		{ "triggerpass", '8', &ChatHandler::HandleTriggerpassCheatCommand, "Ignores area trigger prerequisites.",             NULL, 0, 0, 0 },
		{ "triggerpass", '9', &ChatHandler::HandleTriggerpassCheatCommand, "Ignores area trigger prerequisites.",             NULL, 0, 0, 0 },
		{ NULL,          '0', NULL,                                        "",                                                NULL, 0, 0, 0 }
	};
	dupe_command_table(CheatCommandTable, _CheatCommandTable);

	static ChatCommand accountCommandTable[] =
	{
		{ "level",  '9', &ChatHandler::HandleAccountLevelCommand,  "Sets 2 level on account. Pass it username and 0,1,2,5,az, etc.", NULL, 0, 0, 0 },
		{ "mute",   '2', &ChatHandler::HandleAccountMuteCommand,   "Mutes account for <timeperiod>.",                                 NULL, 0, 0, 0 },
		{ "mute",   '5', &ChatHandler::HandleAccountMuteCommand,   "Mutes account for <timeperiod>.",                                 NULL, 0, 0, 0 },
		{ "mute",   '8', &ChatHandler::HandleAccountMuteCommand,   "Mutes account for <timeperiod>.",                                 NULL, 0, 0, 0 },
		{ "mute",   '9', &ChatHandler::HandleAccountMuteCommand,   "Mutes account for <timeperiod>.",                                 NULL, 0, 0, 0 },
		{ "unmute", '2', &ChatHandler::HandleAccountUnmuteCommand, "Unmutes account <x>",                                             NULL, 0, 0, 0 },
		{ "unmute", '5', &ChatHandler::HandleAccountUnmuteCommand, "Unmutes account <x>",                                             NULL, 0, 0, 0 },
		{ "unmute", '8', &ChatHandler::HandleAccountUnmuteCommand, "Unmutes account <x>",                                             NULL, 0, 0, 0 },
		{ "unmute", '9', &ChatHandler::HandleAccountUnmuteCommand, "Unmutes account <x>",                                             NULL, 0, 0, 0 },
		{ NULL,     '0', NULL,                                     "",                                                                NULL, 0, 0, 0 }
	};
	dupe_command_table(accountCommandTable, _accountCommandTable);

	static ChatCommand honorCommandTable[] =
	{
		{ "addpoints",         '8', &ChatHandler::HandleAddHonorCommand,                    "Adds x amount of honor points/currency",                  NULL, 0, 0, 0 },
		{ "addpoints",         '9', &ChatHandler::HandleAddHonorCommand,                    "Adds x amount of honor points/currency",                  NULL, 0, 0, 0 },
		{ "addkills",          '8', &ChatHandler::HandleAddKillCommand,                     "Adds x amount of honor kills",                            NULL, 0, 0, 0 },
		{ "addkills",          '9', &ChatHandler::HandleAddKillCommand,                     "Adds x amount of honor kills",                            NULL, 0, 0, 0 },
		{ "globaldailyupdate", '8', &ChatHandler::HandleGlobalHonorDailyMaintenanceCommand, "Daily honor field moves",                                 NULL, 0, 0, 0 },
		{ "globaldailyupdate", '9', &ChatHandler::HandleGlobalHonorDailyMaintenanceCommand, "Daily honor field moves",                                 NULL, 0, 0, 0 },
		{ "singledailyupdate", '8', &ChatHandler::HandleNextDayCommand,                     "Daily honor field moves for selected player only",        NULL, 0, 0, 0 },
		{ "singledailyupdate", '9', &ChatHandler::HandleNextDayCommand,                     "Daily honor field moves for selected player only",        NULL, 0, 0, 0 },
		{ "pvpcredit",         '8', &ChatHandler::HandlePVPCreditCommand,                   "Sends PVP credit packet, with specified rank and points", NULL, 0, 0, 0 },
		{ "pvpcredit",         '9', &ChatHandler::HandlePVPCreditCommand,                   "Sends PVP credit packet, with specified rank and points", NULL, 0, 0, 0 },
		{ NULL,                '0', NULL,                                                   "",                                                        NULL, 0, 0, 0 }
	};
	dupe_command_table(honorCommandTable, _honorCommandTable);

	static ChatCommand petCommandTable[] =
	{
		{ "createpet",   '8', &ChatHandler::HandleCreatePetCommand,      "Creates a pet with <entry>.",                            NULL, 0, 0, 0 },
		{ "createpet",   '9', &ChatHandler::HandleCreatePetCommand,      "Creates a pet with <entry>.",                            NULL, 0, 0, 0 },
		{ "dismiss",     '8', &ChatHandler::HandleDismissPetCommand,     "Dismisses selected pet.",                                NULL, 0, 0, 0 },
		{ "dismiss",     '9', &ChatHandler::HandleDismissPetCommand,     "Dismisses selected pet.",                                NULL, 0, 0, 0 },
		{ "renamepet",   '8', &ChatHandler::HandleRenamePetCommand,      "Renames a pet to <name>.",                               NULL, 0, 0, 0 },
		{ "renamepet",   '9', &ChatHandler::HandleRenamePetCommand,      "Renames a pet to <name>.",                               NULL, 0, 0, 0 },
		{ "addspell",    '8', &ChatHandler::HandleAddPetSpellCommand,    "Teaches pet <spell>.",                                   NULL, 0, 0, 0 },
		{ "addspell",    '9', &ChatHandler::HandleAddPetSpellCommand,    "Teaches pet <spell>.",                                   NULL, 0, 0, 0 },
		{ "removespell", '8', &ChatHandler::HandleRemovePetSpellCommand, "Removes pet spell <spell>.",                             NULL, 0, 0, 0 },
		{ "removespell", '9', &ChatHandler::HandleRemovePetSpellCommand, "Removes pet spell <spell>.",                             NULL, 0, 0, 0 },
		{ "setlevel",    '8', &ChatHandler::HandlePetLevelCommand,       "Sets pet level to <level>.",                             NULL, 0, 0, 0 },
		{ "setlevel",    '9', &ChatHandler::HandlePetLevelCommand,       "Sets pet level to <level>.",                             NULL, 0, 0, 0 },
#ifdef USE_SPECIFIC_AIAGENTS
		{ "spawnbot",    'a', &ChatHandler::HandlePetSpawnAIBot,         ".pet spawnbot <type> - spawn a helper bot for your aid", NULL, 0, 0, 0 },
#endif
		{ NULL,          '0', NULL,                                      "",                                                       NULL, 0, 0, 0 }
	};
	dupe_command_table(petCommandTable, _petCommandTable);

	static ChatCommand recallCommandTable[] =
	{
		{ "list",       '2', &ChatHandler::HandleRecallListCommand,       "List recall locations",     NULL, 0, 0, 0 },
		{ "list",       '5', &ChatHandler::HandleRecallListCommand,       "List recall locations",     NULL, 0, 0, 0 },
		{ "list",       '8', &ChatHandler::HandleRecallListCommand,       "List recall locations",     NULL, 0, 0, 0 },
		{ "list",       '9', &ChatHandler::HandleRecallListCommand,       "List recall locations",     NULL, 0, 0, 0 },
		{ "add",        '2', &ChatHandler::HandleRecallAddCommand,        "Add a recall location",       NULL, 0, 0, 0 },
		{ "add",        '5', &ChatHandler::HandleRecallAddCommand,        "Add a recall location",       NULL, 0, 0, 0 },
		{ "add",        '8', &ChatHandler::HandleRecallAddCommand,        "Add a recall location",       NULL, 0, 0, 0 },
		{ "add",        '9', &ChatHandler::HandleRecallAddCommand,        "Add a recall location",       NULL, 0, 0, 0 },
		{ "del",        '2', &ChatHandler::HandleRecallDelCommand,        "Remove a recall location",  NULL, 0, 0, 0 },
		{ "del",        '5', &ChatHandler::HandleRecallDelCommand,        "Remove a recall location",  NULL, 0, 0, 0 },
		{ "del",        '8', &ChatHandler::HandleRecallDelCommand,        "Remove a recall location",  NULL, 0, 0, 0 },
		{ "del",        '9', &ChatHandler::HandleRecallDelCommand,        "Remove a recall location",  NULL, 0, 0, 0 },
		{ "port",       '2', &ChatHandler::HandleRecallGoCommand,         "Ports you to recalled location", NULL, 0, 0, 0 },
		{ "port",       '5', &ChatHandler::HandleRecallGoCommand,         "Ports you to recalled location", NULL, 0, 0, 0 },
		{ "port",       '8', &ChatHandler::HandleRecallGoCommand,         "Ports you to recalled location", NULL, 0, 0, 0 },
		{ "port",       '9', &ChatHandler::HandleRecallGoCommand,         "Ports you to recalled location", NULL, 0, 0, 0 },
		{ "portplayer", '2', &ChatHandler::HandleRecallPortPlayerCommand, "Ports specified player to a recalled location", NULL, 0, 0, 0 },
		{ "portplayer", '5', &ChatHandler::HandleRecallPortPlayerCommand, "Ports specified player to a recalled location", NULL, 0, 0, 0 },
		{ "portplayer", '8', &ChatHandler::HandleRecallPortPlayerCommand, "Ports specified player to a recalled location", NULL, 0, 0, 0 },
		{ "portplayer", '9', &ChatHandler::HandleRecallPortPlayerCommand, "Ports specified player to a recalled location", NULL, 0, 0, 0 },
		{ "portus",		'2', &ChatHandler::HandleRecallPortUsCommand,	  "Ports you and the selected player to recalled location",       NULL, 0, 0, 0 },
		{ "portus",		'5', &ChatHandler::HandleRecallPortUsCommand,	  "Ports you and the selected player to recalled location",       NULL, 0, 0, 0 },
		{ "portus",		'8', &ChatHandler::HandleRecallPortUsCommand,	  "Ports you and the selected player to recalled location",       NULL, 0, 0, 0 },
		{ "portus",		'9', &ChatHandler::HandleRecallPortUsCommand,	  "Ports you and the selected player to recalled location",       NULL, 0, 0, 0 },
		{ NULL,         '0', NULL,                                        "",                          NULL, 0, 0, 0 }
	};
	dupe_command_table(recallCommandTable, _recallCommandTable);

	static ChatCommand questCommandTable[] =
	{
		{ "addboth",   '8', &ChatHandler::HandleQuestAddBothCommand,   "Add quest <id> to the targeted NPC as start & finish",      NULL, 0, 0, 0 },
		{ "addboth",   '9', &ChatHandler::HandleQuestAddBothCommand,   "Add quest <id> to the targeted NPC as start & finish",      NULL, 0, 0, 0 },
		{ "addfinish", '8', &ChatHandler::HandleQuestAddFinishCommand, "Add quest <id> to the targeted NPC as finisher",            NULL, 0, 0, 0 },
		{ "addfinish", '9', &ChatHandler::HandleQuestAddFinishCommand, "Add quest <id> to the targeted NPC as finisher",            NULL, 0, 0, 0 },
		{ "addstart",  '8', &ChatHandler::HandleQuestAddStartCommand,  "Add quest <id> to the targeted NPC as starter",             NULL, 0, 0, 0 },
		{ "addstart",  '9', &ChatHandler::HandleQuestAddStartCommand,  "Add quest <id> to the targeted NPC as starter",             NULL, 0, 0, 0 },
		{ "delboth",   '8', &ChatHandler::HandleQuestDelBothCommand,   "Delete quest <id> from the targeted NPC as start & finish", NULL, 0, 0, 0 },
		{ "delboth",   '9', &ChatHandler::HandleQuestDelBothCommand,   "Delete quest <id> from the targeted NPC as start & finish", NULL, 0, 0, 0 },
		{ "delfinish", '8', &ChatHandler::HandleQuestDelFinishCommand, "Delete quest <id> from the targeted NPC as finisher",       NULL, 0, 0, 0 },
		{ "delfinish", '9', &ChatHandler::HandleQuestDelFinishCommand, "Delete quest <id> from the targeted NPC as finisher",       NULL, 0, 0, 0 },
		{ "delstart",  '8', &ChatHandler::HandleQuestDelStartCommand,  "Delete quest <id> from the targeted NPC as starter",        NULL, 0, 0, 0 },
		{ "delstart",  '9', &ChatHandler::HandleQuestDelStartCommand,  "Delete quest <id> from the targeted NPC as starter",        NULL, 0, 0, 0 },
		{ "complete",  '8', &ChatHandler::HandleQuestFinishCommand,    "Complete/Finish quest <id>",                                NULL, 0, 0, 0 },
		{ "complete",  '9', &ChatHandler::HandleQuestFinishCommand,    "Complete/Finish quest <id>",                                NULL, 0, 0, 0 },
		{ "fail",      '8', &ChatHandler::HandleQuestFailCommand,      "Fail quest <id>",                                           NULL, 0, 0, 0 },
		{ "fail",      '9', &ChatHandler::HandleQuestFailCommand,      "Fail quest <id>",                                           NULL, 0, 0, 0 },
		{ "finisher",  '8', &ChatHandler::HandleQuestFinisherCommand,  "Lookup quest finisher for quest <id>",                      NULL, 0, 0, 0 },
		{ "finisher",  '9', &ChatHandler::HandleQuestFinisherCommand,  "Lookup quest finisher for quest <id>",                      NULL, 0, 0, 0 },
		{ "item",      '8', &ChatHandler::HandleQuestItemCommand,      "Lookup itemid necessary for quest <id>",                    NULL, 0, 0, 0 },
		{ "item",      '9', &ChatHandler::HandleQuestItemCommand,      "Lookup itemid necessary for quest <id>",                    NULL, 0, 0, 0 },
		{ "list",      '8', &ChatHandler::HandleQuestListCommand,      "Lists the quests for the npc <id>",                         NULL, 0, 0, 0 },
		{ "list",      '9', &ChatHandler::HandleQuestListCommand,      "Lists the quests for the npc <id>",                         NULL, 0, 0, 0 },
		{ "load",      '8', &ChatHandler::HandleQuestLoadCommand,      "Loads quests from database",                                NULL, 0, 0, 0 },
		{ "load",      '9', &ChatHandler::HandleQuestLoadCommand,      "Loads quests from database",                                NULL, 0, 0, 0 },
		{ "lookup",    '8', &ChatHandler::HandleQuestLookupCommand,    "Looks up quest string x",                                   NULL, 0, 0, 0 },
		{ "lookup",    '9', &ChatHandler::HandleQuestLookupCommand,    "Looks up quest string x",                                   NULL, 0, 0, 0 },
		{ "giver",     '8', &ChatHandler::HandleQuestGiverCommand,     "Lookup quest giver for quest <id>",                         NULL, 0, 0, 0 },
		{ "giver",     '9', &ChatHandler::HandleQuestGiverCommand,     "Lookup quest giver for quest <id>",                         NULL, 0, 0, 0 },
		{ "remove",    '8', &ChatHandler::HandleQuestRemoveCommand,    "Removes the quest <id> from the targeted player",           NULL, 0, 0, 0 },
		{ "remove",    '9', &ChatHandler::HandleQuestRemoveCommand,    "Removes the quest <id> from the targeted player",           NULL, 0, 0, 0 },
		{ "reward",    '8', &ChatHandler::HandleQuestRewardCommand,    "Shows reward for quest <id>",                               NULL, 0, 0, 0 },
		{ "reward",    '9', &ChatHandler::HandleQuestRewardCommand,    "Shows reward for quest <id>",                               NULL, 0, 0, 0 },
		{ "status",    '8', &ChatHandler::HandleQuestStatusCommand,    "Lists the status of quest <id>",                            NULL, 0, 0, 0 },
		{ "status",    '9', &ChatHandler::HandleQuestStatusCommand,    "Lists the status of quest <id>",                            NULL, 0, 0, 0 },
		{ "start",     '8', &ChatHandler::HandleQuestStartCommand,     "Starts quest <id>",                                         NULL, 0, 0, 0 },
		{ "start",     '9', &ChatHandler::HandleQuestStartCommand,     "Starts quest <id>",                                         NULL, 0, 0, 0 },
		{ "startspawn", '8', &ChatHandler::HandleQuestStarterSpawnCommand, "Port to spawn location for quest <id> (starter)",        NULL, 0, 0, 0 },
		{ "startspawn", '9', &ChatHandler::HandleQuestStarterSpawnCommand, "Port to spawn location for quest <id> (starter)",        NULL, 0, 0, 0 },
		{ "finishspawn", '8', &ChatHandler::HandleQuestFinisherSpawnCommand, "Port to spawn location for quest <id> (finisher)",       NULL, 0, 0, 0 },
		{ "finishspawn", '9', &ChatHandler::HandleQuestFinisherSpawnCommand, "Port to spawn location for quest <id> (finisher)",       NULL, 0, 0, 0 },
		{ NULL,        '0', NULL,                                      "",                                                          NULL, 0, 0, 0 }
	};
	dupe_command_table(questCommandTable, _questCommandTable);

	static ChatCommand serverCommandTable[] =
	{
		{ "setmotd",       '5', &ChatHandler::HandleSetMotdCommand,         "Sets MOTD",                                                NULL, 0, 0, 0 },
		{ "setmotd",       '8', &ChatHandler::HandleSetMotdCommand,         "Sets MOTD",                                                NULL, 0, 0, 0 },
		{ "setmotd",       '9', &ChatHandler::HandleSetMotdCommand,         "Sets MOTD",                                                NULL, 0, 0, 0 },
		{ "rehash",        '8', &ChatHandler::HandleRehashCommand,          "Reloads config file.",                                     NULL, 0, 0, 0 },
		{ "rehash",        '9', &ChatHandler::HandleRehashCommand,          "Reloads config file.",                                     NULL, 0, 0, 0 },
		//{ "reloadscripts", 'w', &ChatHandler::HandleReloadScriptsCommand,   "Reloads 2 Scripts",                                       NULL, 0, 0, 0 },
		{ "reloadtable",   '8', &ChatHandler::HandleDBReloadCommand,        "",                      NULL, 0, 0, 0 },
		{ "reloadtable",   '9', &ChatHandler::HandleDBReloadCommand,        "",                      NULL, 0, 0, 0 },
		{ "shutdown",      '8', &ChatHandler::HandleShutdownCommand,        "Initiates server shutdown in <x> seconds (8 by default).", NULL, 0, 0, 0 },
		{ "shutdown",      '9', &ChatHandler::HandleShutdownCommand,        "Initiates server shutdown in <x> seconds (8 by default).", NULL, 0, 0, 0 },
		{ "restart",       '8', &ChatHandler::HandleShutdownRestartCommand, "Initiates server restart in <x> seconds (8 by default).",  NULL, 0, 0, 0 },
		{ "restart",       '9', &ChatHandler::HandleShutdownRestartCommand, "Initiates server restart in <x> seconds (8 by default).",  NULL, 0, 0, 0 },
		{ "cancelshutdown", '8', &ChatHandler::HandleCancelShutdownCommand,  "Cancels a Server Restart/Shutdown.",						NULL, 0, 0, 0 },
		{ "cancelshutdown", '9', &ChatHandler::HandleCancelShutdownCommand,  "Cancels a Server Restart/Shutdown.",						NULL, 0, 0, 0 },
		{ "save",          '0', &ChatHandler::HandleSaveCommand,            "Save's target character",                                    NULL, 0, 0, 0 },
		{ "saveall",       '2', &ChatHandler::HandleSaveAllCommand,         "Save's all playing characters",                            NULL, 0, 0, 0 },
		{ "saveall",       '5', &ChatHandler::HandleSaveAllCommand,         "Save's all playing characters",                            NULL, 0, 0, 0 },
		{ "saveall",       '8', &ChatHandler::HandleSaveAllCommand,         "Save's all playing characters",                            NULL, 0, 0, 0 },
		{ "saveall",       '9', &ChatHandler::HandleSaveAllCommand,         "Save's all playing characters",                            NULL, 0, 0, 0 },
		{ "info",          '0', &ChatHandler::HandleInfoCommand,            "Server info",                                              NULL, 0, 0, 0 },
		{ "netstatus",     '8', &ChatHandler::HandleNetworkStatusCommand,   "Shows network status.", NULL, 0, 0, 0 },
		{ "netstatus",     '9', &ChatHandler::HandleNetworkStatusCommand,   "Shows network status.", NULL, 0, 0, 0 },
		{ NULL,            '0', NULL,                                       "",                                                         NULL, 0, 0, 0 }
	};
	dupe_command_table(serverCommandTable, _serverCommandTable);

	static ChatCommand gmCommandTable[] =
	{
		{ "list",          '0', &ChatHandler::HandleGMListCommand,        "Shows active 2's",                                   NULL, 0, 0, 0 },
		{ "status",        '2', &ChatHandler::HandleGMStatusCommand,      "Shows status of your 2 flags",             	    	 NULL, 0, 0, 0 },
		{ "status",        '5', &ChatHandler::HandleGMStatusCommand,      "Shows status of your 2 flags",             	    	 NULL, 0, 0, 0 },
		{ "status",        '8', &ChatHandler::HandleGMStatusCommand,      "Shows status of your 2 flags",             	    	 NULL, 0, 0, 0 },
		{ "status",        '9', &ChatHandler::HandleGMStatusCommand,      "Shows status of your 2 flags",             	    	 NULL, 0, 0, 0 },
		{ "off",           '2', &ChatHandler::HandleGMOffCommand,         "Sets 2 tag off",                                     NULL, 0, 0, 0 },
		{ "off",           '5', &ChatHandler::HandleGMOffCommand,         "Sets 2 tag off",                                     NULL, 0, 0, 0 },
		{ "off",           '8', &ChatHandler::HandleGMOffCommand,         "Sets 2 tag off",                                     NULL, 0, 0, 0 },
		{ "off",           '9', &ChatHandler::HandleGMOffCommand,         "Sets 2 tag off",                                     NULL, 0, 0, 0 },
		{ "on",            '2', &ChatHandler::HandleGMOnCommand,          "Sets 2 tag on",                                      NULL, 0, 0, 0 },
		{ "on",            '5', &ChatHandler::HandleGMOnCommand,          "Sets 2 tag on",                                      NULL, 0, 0, 0 },
		{ "on",            '8', &ChatHandler::HandleGMOnCommand,          "Sets 2 tag on",                                      NULL, 0, 0, 0 },
		{ "on",            '9', &ChatHandler::HandleGMOnCommand,          "Sets 2 tag on",                                      NULL, 0, 0, 0 },
		{ "whisperblock",  '2', &ChatHandler::HandleWhisperBlockCommand,  "Blocks like .gmon except without the <2> tag",       NULL, 0, 0, 0 },
		{ "whisperblock",  '5', &ChatHandler::HandleWhisperBlockCommand,  "Blocks like .gmon except without the <2> tag",       NULL, 0, 0, 0 },
		{ "whisperblock",  '8', &ChatHandler::HandleWhisperBlockCommand,  "Blocks like .gmon except without the <2> tag",       NULL, 0, 0, 0 },
		{ "whisperblock",  '9', &ChatHandler::HandleWhisperBlockCommand,  "Blocks like .gmon except without the <2> tag",       NULL, 0, 0, 0 },
		{ "allowwhispers", '2', &ChatHandler::HandleAllowWhispersCommand, "Allows whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "allowwhispers", '5', &ChatHandler::HandleAllowWhispersCommand, "Allows whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "allowwhispers", '8', &ChatHandler::HandleAllowWhispersCommand, "Allows whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "allowwhispers", '9', &ChatHandler::HandleAllowWhispersCommand, "Allows whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "blockwhispers", '2', &ChatHandler::HandleBlockWhispersCommand, "Blocks whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "blockwhispers", '5', &ChatHandler::HandleBlockWhispersCommand, "Blocks whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "blockwhispers", '8', &ChatHandler::HandleBlockWhispersCommand, "Blocks whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ "blockwhispers", '9', &ChatHandler::HandleBlockWhispersCommand, "Blocks whispers from player <s> while in gmon mode.", NULL, 0, 0, 0 },
		{ NULL,            '0', NULL,                                     "",                                                    NULL, 0, 0, 0 }
	};
	dupe_command_table(gmCommandTable, _gmCommandTable);

	static ChatCommand characterCommandTable[] =
	{
		{ "learn",               '8', &ChatHandler::HandleLearnCommand,            "Learns spell",                                                                                                      NULL, 0, 0, 0 },
		{ "learn",               '9', &ChatHandler::HandleLearnCommand,            "Learns spell",                                                                                                      NULL, 0, 0, 0 },
		{ "unlearn",             '8', &ChatHandler::HandleUnlearnCommand,          "Unlearns spell",                                                                                                    NULL, 0, 0, 0 },
		{ "unlearn",             '9', &ChatHandler::HandleUnlearnCommand,          "Unlearns spell",                                                                                                    NULL, 0, 0, 0 },
		{ "getskillinfo",        '8', &ChatHandler::HandleGetSkillsInfoCommand,    "Gets all the skills from a player",                                                                                 NULL, 0, 0, 0 },
		{ "getskillinfo",        '9', &ChatHandler::HandleGetSkillsInfoCommand,    "Gets all the skills from a player",                                                                                 NULL, 0, 0, 0 },
		{ "learnskill",          '8', &ChatHandler::HandleLearnSkillCommand,       ".learnskill <skillid> (optional) <value> <maxvalue> - Learns skill id skillid.",                                    NULL, 0, 0, 0 },
		{ "learnskill",          '9', &ChatHandler::HandleLearnSkillCommand,       ".learnskill <skillid> (optional) <value> <maxvalue> - Learns skill id skillid.",                                    NULL, 0, 0, 0 },
		{ "advanceskill",        '8', &ChatHandler::HandleModifySkillCommand,      "advanceskill <skillid> <amount, optional, default = 1> - Advances skill line x times..",                            NULL, 0, 0, 0 },
		{ "advanceskill",        '9', &ChatHandler::HandleModifySkillCommand,      "advanceskill <skillid> <amount, optional, default = 1> - Advances skill line x times..",                            NULL, 0, 0, 0 },
		{ "removeskill",         '8', &ChatHandler::HandleRemoveSkillCommand,      ".removeskill <skillid> - Removes skill",                                                                            NULL, 0, 0, 0 },
		{ "removeskill",         '9', &ChatHandler::HandleRemoveSkillCommand,      ".removeskill <skillid> - Removes skill",                                                                            NULL, 0, 0, 0 },
		{ "increaseweaponskill", '8', &ChatHandler::HandleIncreaseWeaponSkill,     ".increaseweaponskill <count> - Increase equipped weapon skill x times (defaults to 1).",                             NULL, 0, 0, 0 },
		{ "increaseweaponskill", '9', &ChatHandler::HandleIncreaseWeaponSkill,     ".increaseweaponskill <count> - Increase equipped weapon skill x times (defaults to 1).",                             NULL, 0, 0, 0 },
		{ "resetreputation",     '8', &ChatHandler::HandleResetReputationCommand,  ".resetreputation - Resets reputation to start levels. (use on characters that were made before reputation fixes.)", NULL, 0, 0, 0 },
		{ "resetreputation",     '9', &ChatHandler::HandleResetReputationCommand,  ".resetreputation - Resets reputation to start levels. (use on characters that were made before reputation fixes.)", NULL, 0, 0, 0 },
		{ "resetspells",         '8', &ChatHandler::HandleResetSpellsCommand,      ".resetspells - Resets all spells to starting spells of targeted player. DANGEROUS.",                                NULL, 0, 0, 0 },
		{ "resetspells",         '9', &ChatHandler::HandleResetSpellsCommand,      ".resetspells - Resets all spells to starting spells of targeted player. DANGEROUS.",                                NULL, 0, 0, 0 },
		{ "resettalents",        '5', &ChatHandler::HandleResetTalentsCommand,     ".resettalents - Resets all talents of targeted player to that of their current level. DANGEROUS.",                  NULL, 0, 0, 0 },
		{ "resettalents",        '8', &ChatHandler::HandleResetTalentsCommand,     ".resettalents - Resets all talents of targeted player to that of their current level. DANGEROUS.",                  NULL, 0, 0, 0 },
		{ "resettalents",        '9', &ChatHandler::HandleResetTalentsCommand,     ".resettalents - Resets all talents of targeted player to that of their current level. DANGEROUS.",                  NULL, 0, 0, 0 },
		{ "resetskills",         '8', &ChatHandler::HandleResetSkillsCommand,      ".resetskills - Resets all skills.",                                                                                 NULL, 0, 0, 0 },
		{ "resetskills",         '9', &ChatHandler::HandleResetSkillsCommand,      ".resetskills - Resets all skills.",                                                                                 NULL, 0, 0, 0 },
		{ "additem",             '9', &ChatHandler::HandleAddInvItemCommand,       "Adds item x count y",                                                                                                                  NULL, 0, 0, 0 },
		{ "removeitem",          '9', &ChatHandler::HandleRemoveItemCommand,       "Removes item x count y.",                                                                                         NULL, 0, 0, 0 },
		{ "additemset",          '9', &ChatHandler::HandleAddItemSetCommand,       "Adds item set to inv.",                                                                                             NULL, 0, 0, 0 },
		{ "advanceallskills",    '8', &ChatHandler::HandleAdvanceAllSkillsCommand, "Advances all skills <x> points.",                                                                                   NULL, 0, 0, 0 },
		{ "advanceallskills",    '9', &ChatHandler::HandleAdvanceAllSkillsCommand, "Advances all skills <x> points.",                                                                                   NULL, 0, 0, 0 },
		{ "getstanding",         '8', &ChatHandler::HandleGetStandingCommand,      "Gets standing of faction x.",                                                                                      NULL, 0, 0, 0 },
		{ "getstanding",         '9', &ChatHandler::HandleGetStandingCommand,      "Gets standing of faction x.",                                                                                      NULL, 0, 0, 0 },
		{ "setstanding",         '8', &ChatHandler::HandleSetStandingCommand,      "Sets standing of faction x.",                                                                                      NULL, 0, 0, 0 },
		{ "setstanding",         '9', &ChatHandler::HandleSetStandingCommand,      "Sets standing of faction x.",                                                                                      NULL, 0, 0, 0 },
		{ "showitems",           '2', &ChatHandler::HandleShowItems,               "Shows items of selected Player",                                                                                    NULL, 0, 0, 0 },
		{ "showitems",           '5', &ChatHandler::HandleShowItems,               "Shows items of selected Player",                                                                                    NULL, 0, 0, 0 },
		{ "showitems",           '8', &ChatHandler::HandleShowItems,               "Shows items of selected Player",                                                                                    NULL, 0, 0, 0 },
		{ "showitems",           '9', &ChatHandler::HandleShowItems,               "Shows items of selected Player",                                                                                    NULL, 0, 0, 0 },
		{ "showskills",          '2', &ChatHandler::HandleShowSkills,              "Shows skills of selected Player",                                                                                   NULL, 0, 0, 0 },
		{ "showskills",          '5', &ChatHandler::HandleShowSkills,              "Shows skills of selected Player",                                                                                   NULL, 0, 0, 0 },
		{ "showskills",          '8', &ChatHandler::HandleShowSkills,              "Shows skills of selected Player",                                                                                   NULL, 0, 0, 0 },
		{ "showskills",          '9', &ChatHandler::HandleShowSkills,              "Shows skills of selected Player",                                                                                   NULL, 0, 0, 0 },
		{ "showinstances",       '8', &ChatHandler::HandleShowInstancesCommand,    "Shows persistent instances of selected Player",                                                                     NULL, 0, 0, 0 },
		{ "showinstances",       '9', &ChatHandler::HandleShowInstancesCommand,    "Shows persistent instances of selected Player",                                                                     NULL, 0, 0, 0 },
		{ "rename",              '5', &ChatHandler::HandleRenameCommand,           "Renames character x to y.",                                                                                         NULL, 0, 0, 0 },
		{ "rename",              '8', &ChatHandler::HandleRenameCommand,           "Renames character x to y.",                                                                                         NULL, 0, 0, 0 },
		{ "rename",              '9', &ChatHandler::HandleRenameCommand,           "Renames character x to y.",                                                                                         NULL, 0, 0, 0 },
		{ "forcerename",         '5', &ChatHandler::HandleForceRenameCommand,      "Forces character x to rename his char next login",                                                                  NULL, 0, 0, 0 },
		{ "forcerename",         '8', &ChatHandler::HandleForceRenameCommand,      "Forces character x to rename his char next login",                                                                  NULL, 0, 0, 0 },
		{ "forcerename",         '9', &ChatHandler::HandleForceRenameCommand,      "Forces character x to rename his char next login",                                                                  NULL, 0, 0, 0 },
		{ "repairitems",         '8', &ChatHandler::HandleRepairItemsCommand,      ".repairitems - Repair all items from selected player",                                                              NULL, 0, 0, 0 },
		{ "repairitems",         '9', &ChatHandler::HandleRepairItemsCommand,      ".repairitems - Repair all items from selected player",                                                              NULL, 0, 0, 0 },
		{ "settitle",			 '5', &ChatHandler::HandleSetTitle,				   "Adds title to a player",																					NULL, 0, 0, 0 },
		{ "settitle",			 '8', &ChatHandler::HandleSetTitle,				   "Adds title to a player",																					NULL, 0, 0, 0 },
		{ "settitle",			 '9', &ChatHandler::HandleSetTitle,				   "Adds title to a player",																					NULL, 0, 0, 0 },
		{ "phase",               '8', &ChatHandler::HandlePhaseCommand,            "<phase> - Sets phase of selected player",                                                                           NULL, 0, 0, 0 },
		{ "phase",               '9', &ChatHandler::HandlePhaseCommand,            "<phase> - Sets phase of selected player",                                                                           NULL, 0, 0, 0 },
		{ NULL,                  '0', NULL,                                        "",                                                                                                                  NULL, 0, 0, 0 }
	};
	dupe_command_table(characterCommandTable, _characterCommandTable);

	static ChatCommand lookupCommandTable[] =
	{
		{ "item",     '5', &ChatHandler::HandleLookupItemCommand,     "Looks up item string x.",  NULL, 0, 0, 0 },
		{ "item",     '8', &ChatHandler::HandleLookupItemCommand,     "Looks up item string x.",  NULL, 0, 0, 0 },
		{ "item",     '9', &ChatHandler::HandleLookupItemCommand,     "Looks up item string x.",  NULL, 0, 0, 0 },
		{ "quest",    '8', &ChatHandler::HandleQuestLookupCommand,    "Looks up quest string x.", NULL, 0, 0, 0 },
		{ "quest",    '9', &ChatHandler::HandleQuestLookupCommand,    "Looks up quest string x.", NULL, 0, 0, 0 },
		{ "creature", '8', &ChatHandler::HandleLookupCreatureCommand, "Looks up item string x.",  NULL, 0, 0, 0 },
		{ "creature", '9', &ChatHandler::HandleLookupCreatureCommand, "Looks up item string x.",  NULL, 0, 0, 0 },
		{ "object",   '2', &ChatHandler::HandleLookupObjectCommand,   "Looks up gameobject string x.", NULL, 0, 0 , 0},
		{ "object",   '5', &ChatHandler::HandleLookupObjectCommand,   "Looks up gameobject string x.", NULL, 0, 0 , 0},
		{ "object",   '8', &ChatHandler::HandleLookupObjectCommand,   "Looks up gameobject string x.", NULL, 0, 0 , 0},
		{ "object",   '9', &ChatHandler::HandleLookupObjectCommand,   "Looks up gameobject string x.", NULL, 0, 0 , 0},
		{ "spell",    '8', &ChatHandler::HandleLookupSpellCommand,    "Looks up spell string x.", NULL, 0, 0, 0 },
		{ "spell",    '9', &ChatHandler::HandleLookupSpellCommand,    "Looks up spell string x.", NULL, 0, 0, 0 },
		{ "skill",    '8', &ChatHandler::HandleLookupSkillCommand,    "Looks up skill string x.", NULL, 0, 0, 0 },
		{ "skill",    '9', &ChatHandler::HandleLookupSkillCommand,    "Looks up skill string x.", NULL, 0, 0, 0 },
		{ "faction",  '8', &ChatHandler::HandleLookupFactionCommand,  "Looks up faction string x.", NULL, 0, 0, 0 },
		{ "faction",  '9', &ChatHandler::HandleLookupFactionCommand,  "Looks up faction string x.", NULL, 0, 0, 0 },
#ifdef ENABLE_ACHIEVEMENTS
		{ "achievement", '2', &ChatHandler::HandleLookupAchievementCmd,  "Looks up achievement string x.", NULL, 0, 0, 0 },
		{ "achievement", '5', &ChatHandler::HandleLookupAchievementCmd,  "Looks up achievement string x.", NULL, 0, 0, 0 },
		{ "achievement", '8', &ChatHandler::HandleLookupAchievementCmd,  "Looks up achievement string x.", NULL, 0, 0, 0 },
		{ "achievement", '9', &ChatHandler::HandleLookupAchievementCmd,  "Looks up achievement string x.", NULL, 0, 0, 0 },
#endif
		{ NULL,          '0', NULL,                                      "",                               NULL, 0, 0, 0 }
	};
	dupe_command_table(lookupCommandTable, _lookupCommandTable);

	static ChatCommand adminCommandTable[] =
	{
		{ "castall",               '9', &ChatHandler::HandleCastAllCommand,         "Makes all players online cast spell <x>.",                      NULL, 0, 0, 0 },
		{ "dispelall",             '9', &ChatHandler::HandleDispelAllCommand,       "Dispels all negative (or positive w/ 1) auras on all players.", NULL, 0, 0, 0 },
		{ "renameallinvalidchars", '9', &ChatHandler::HandleRenameAllCharacter,     "Renames all invalid character names",                           NULL, 0, 0, 0 },
		{ "masssummon",            '2', &ChatHandler::HandleMassSummonCommand,      "Summons all online players to your location,add the a/A parameter for alliance or h/H for horde.",                            NULL, 0, 0, 0 },
		{ "masssummon",            '5', &ChatHandler::HandleMassSummonCommand,      "Summons all online players to your location,add the a/A parameter for alliance or h/H for horde.",                            NULL, 0, 0, 0 },
		{ "masssummon",            '8', &ChatHandler::HandleMassSummonCommand,      "Summons all online players to your location,add the a/A parameter for alliance or h/H for horde.",                            NULL, 0, 0, 0 },
		{ "masssummon",            '9', &ChatHandler::HandleMassSummonCommand,      "Summons all online players to your location,add the a/A parameter for alliance or h/H for horde.",                            NULL, 0, 0, 0 },
		{ "playall",               '9', &ChatHandler::HandleGlobalPlaySoundCommand, "Plays a sound to everyone on the realm.",                       NULL, 0, 0, 0 },
		{ NULL,                    '0', NULL,                                       "",                                                              NULL, 0, 0, 0 }
	};
	dupe_command_table(adminCommandTable, _adminCommandTable);

	static ChatCommand kickCommandTable[] =
	{
		{ "player",  '2', &ChatHandler::HandleKillByPlayerCommand,  "Disconnects the player with name <s>.",          NULL, 0, 0, 0 },
		{ "player",  '5', &ChatHandler::HandleKillByPlayerCommand,  "Disconnects the player with name <s>.",          NULL, 0, 0, 0 },
		{ "player",  '8', &ChatHandler::HandleKillByPlayerCommand,  "Disconnects the player with name <s>.",          NULL, 0, 0, 0 },
		{ "player",  '9', &ChatHandler::HandleKillByPlayerCommand,  "Disconnects the player with name <s>.",          NULL, 0, 0, 0 },
		{ "account", '2', &ChatHandler::HandleKillBySessionCommand, "Disconnects the session with account name <s>.", NULL, 0, 0, 0 },
		{ "account", '5', &ChatHandler::HandleKillBySessionCommand, "Disconnects the session with account name <s>.", NULL, 0, 0, 0 },
		{ "account", '8', &ChatHandler::HandleKillBySessionCommand, "Disconnects the session with account name <s>.", NULL, 0, 0, 0 },
		{ "account", '9', &ChatHandler::HandleKillBySessionCommand, "Disconnects the session with account name <s>.", NULL, 0, 0, 0 },
		{ "ip",      '2', &ChatHandler::HandleKillByIPCommand,      "Disconnects the session with the ip <s>.",       NULL, 0, 0, 0 },
		{ "ip",      '5', &ChatHandler::HandleKillByIPCommand,      "Disconnects the session with the ip <s>.",       NULL, 0, 0, 0 },
		{ "ip",      '8', &ChatHandler::HandleKillByIPCommand,      "Disconnects the session with the ip <s>.",       NULL, 0, 0, 0 },
		{ "ip",      '9', &ChatHandler::HandleKillByIPCommand,      "Disconnects the session with the ip <s>.",       NULL, 0, 0, 0 },
		{ NULL,        '0', NULL,                                     "",                                               NULL, 0, 0, 0 }
	};
	dupe_command_table(kickCommandTable, _kickCommandTable);

	static ChatCommand banCommandTable[] =
	{
		{ "ip",        '2', &ChatHandler::HandleIPBanCommand,         "Adds an address to the IP ban table: .ban ip <address> [duration] [reason]\nDuration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)\nLack of duration results in a permanent ban.", NULL, 0, 0, 0 },
		{ "ip",        '5', &ChatHandler::HandleIPBanCommand,         "Adds an address to the IP ban table: .ban ip <address> [duration] [reason]\nDuration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)\nLack of duration results in a permanent ban.", NULL, 0, 0, 0 },
		{ "ip",        '8', &ChatHandler::HandleIPBanCommand,         "Adds an address to the IP ban table: .ban ip <address> [duration] [reason]\nDuration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)\nLack of duration results in a permanent ban.", NULL, 0, 0, 0 },
		{ "ip",        '9', &ChatHandler::HandleIPBanCommand,         "Adds an address to the IP ban table: .ban ip <address> [duration] [reason]\nDuration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)\nLack of duration results in a permanent ban.", NULL, 0, 0, 0 },
		{ "character", '2', &ChatHandler::HandleBanCharacterCommand,  "Bans character: .ban character <char> [duration] [reason]",                                                                                                                                                                                                                                          NULL, 0, 0, 0 },
		{ "character", '5', &ChatHandler::HandleBanCharacterCommand,  "Bans character: .ban character <char> [duration] [reason]",                                                                                                                                                                                                                                          NULL, 0, 0, 0 },
		{ "character", '8', &ChatHandler::HandleBanCharacterCommand,  "Bans character: .ban character <char> [duration] [reason]",                                                                                                                                                                                                                                          NULL, 0, 0, 0 },
		{ "character", '9', &ChatHandler::HandleBanCharacterCommand,  "Bans character: .ban character <char> [duration] [reason]",                                                                                                                                                                                                                                          NULL, 0, 0, 0 },
		{ "account",   '2', &ChatHandler::HandleAccountBannedCommand, "Bans account: .ban account <name> [duration] [reason]",                                                                                                                                                                                                                                              NULL, 0, 0, 0 },
		{ "account",   '5', &ChatHandler::HandleAccountBannedCommand, "Bans account: .ban account <name> [duration] [reason]",                                                                                                                                                                                                                                              NULL, 0, 0, 0 },
		{ "account",   '8', &ChatHandler::HandleAccountBannedCommand, "Bans account: .ban account <name> [duration] [reason]",                                                                                                                                                                                                                                              NULL, 0, 0, 0 },
		{ "account",   '9', &ChatHandler::HandleAccountBannedCommand, "Bans account: .ban account <name> [duration] [reason]",                                                                                                                                                                                                                                              NULL, 0, 0, 0 },
		{ "all",       '2', &ChatHandler::HandleBanAllCommand,        "Bans account, ip, and character: .ban all <char> [duration] [reason]",                                                                                                                                                                                                                               NULL, 0, 0, 0 },
		{ "all",       '5', &ChatHandler::HandleBanAllCommand,        "Bans account, ip, and character: .ban all <char> [duration] [reason]",                                                                                                                                                                                                                               NULL, 0, 0, 0 },
		{ "all",       '8', &ChatHandler::HandleBanAllCommand,        "Bans account, ip, and character: .ban all <char> [duration] [reason]",                                                                                                                                                                                                                               NULL, 0, 0, 0 },
		{ "all",       '9', &ChatHandler::HandleBanAllCommand,        "Bans account, ip, and character: .ban all <char> [duration] [reason]",                                                                                                                                                                                                                               NULL, 0, 0, 0 },
		{ NULL,        '0', NULL,                                     "",                                                                                                                                                                                                                                                                                                   NULL, 0, 0, 0 }
	};
	dupe_command_table(banCommandTable, _banCommandTable);

	static ChatCommand unbanCommandTable[] =
	{
		{ "ip",        '2', &ChatHandler::HandleIPUnBanCommand,        "Deletes an address from the IP ban table: <address>", NULL, 0, 0, 0 },
		{ "ip",        '5', &ChatHandler::HandleIPUnBanCommand,        "Deletes an address from the IP ban table: <address>", NULL, 0, 0, 0 },
		{ "ip",        '8', &ChatHandler::HandleIPUnBanCommand,        "Deletes an address from the IP ban table: <address>", NULL, 0, 0, 0 },
		{ "ip",        '9', &ChatHandler::HandleIPUnBanCommand,        "Deletes an address from the IP ban table: <address>", NULL, 0, 0, 0 },
		{ "character", '2', &ChatHandler::HandleUnBanCharacterCommand, "Unbans character x",                                  NULL, 0, 0, 0 },
		{ "character", '5', &ChatHandler::HandleUnBanCharacterCommand, "Unbans character x",                                  NULL, 0, 0, 0 },
		{ "character", '8', &ChatHandler::HandleUnBanCharacterCommand, "Unbans character x",                                  NULL, 0, 0, 0 },
		{ "character", '9', &ChatHandler::HandleUnBanCharacterCommand, "Unbans character x",                                  NULL, 0, 0, 0 },
		{ "account",   '5', &ChatHandler::HandleAccountUnbanCommand,   "Unbans account x.",                                   NULL, 0, 0, 0 },
		{ "account",   '8', &ChatHandler::HandleAccountUnbanCommand,   "Unbans account x.",                                   NULL, 0, 0, 0 },
		{ "account",   '9', &ChatHandler::HandleAccountUnbanCommand,   "Unbans account x.",                                   NULL, 0, 0, 0 },
		{ NULL,        '0', NULL,                                      "",                                                    NULL, 0, 0, 0 }
	};
	dupe_command_table(unbanCommandTable, _unbanCommandTable);

	static ChatCommand instanceCommandTable[] =
	{
		{ "create",   '8', &ChatHandler::HandleCreateInstanceCommand,    "Generically instances a map that requires instancing, mapid x y z",		      NULL, 0, 0, 0 },
		{ "create",   '9', &ChatHandler::HandleCreateInstanceCommand,    "Generically instances a map that requires instancing, mapid x y z",		      NULL, 0, 0, 0 },
		{ "reset",    '0', &ChatHandler::HandleResetInstanceCommand,     "Removes instance ID x from target player.",                         NULL, 0, 0, 0 },
		{ "reset",    '2', &ChatHandler::HandleResetInstanceCommand,     "Removes instance ID x from target player.",                         NULL, 0, 0, 0 },
		{ "reset",    '5', &ChatHandler::HandleResetInstanceCommand,     "Removes instance ID x from target player.",                         NULL, 0, 0, 0 },
		{ "reset",    '8', &ChatHandler::HandleResetInstanceCommand,     "Removes instance ID x from target player.",                         NULL, 0, 0, 0 },
		{ "reset",    '9', &ChatHandler::HandleResetInstanceCommand,     "Removes instance ID x from target player.",                         NULL, 0, 0, 0 },
		{ "resetall", '0', &ChatHandler::HandleResetAllInstancesCommand, "Removes all instance IDs from target player.",                      NULL, 0, 0, 0 },
		{ "resetall", '2', &ChatHandler::HandleResetAllInstancesCommand, "Removes all instance IDs from target player.",                      NULL, 0, 0, 0 },
		{ "resetall", '5', &ChatHandler::HandleResetAllInstancesCommand, "Removes all instance IDs from target player.",                      NULL, 0, 0, 0 },
		{ "resetall", '8', &ChatHandler::HandleResetAllInstancesCommand, "Removes all instance IDs from target player.",                      NULL, 0, 0, 0 },
		{ "resetall", '9', &ChatHandler::HandleResetAllInstancesCommand, "Removes all instance IDs from target player.",                      NULL, 0, 0, 0 },
		{ "shutdown", '8', &ChatHandler::HandleShutdownInstanceCommand,  "Shutdown instance with ID x (default is current instance).",        NULL, 0, 0, 0 },
		{ "shutdown", '9', &ChatHandler::HandleShutdownInstanceCommand,  "Shutdown instance with ID x (default is current instance).",        NULL, 0, 0, 0 },
		//{ "delete",   'z', &ChatHandler::HandleDeleteInstanceCommand,    "Deletes instance with ID x (default is current instance).",         NULL, 0, 0, 0 },
		{ "info",     '8', &ChatHandler::HandleGetInstanceInfoCommand,   "Gets info about instance with ID x (default is current instance).", NULL, 0, 0, 0 },
		{ "info",     '9', &ChatHandler::HandleGetInstanceInfoCommand,   "Gets info about instance with ID x (default is current instance).", NULL, 0, 0, 0 },
		{ "exit",     '8', &ChatHandler::HandleExitInstanceCommand,      "Exits current instance, return to entry point.",                    NULL, 0, 0, 0 },
		{ "exit",     '9', &ChatHandler::HandleExitInstanceCommand,      "Exits current instance, return to entry point.",                    NULL, 0, 0, 0 },
		{ NULL,       '0', NULL,                                         "",                                                                  NULL, 0, 0, 0 }
	};
	dupe_command_table(instanceCommandTable, _instanceCommandTable);

	static ChatCommand arenaCommandTable[] =
	{
		{ "createteam",      '8', &ChatHandler::HandleArenaCreateTeamCommand,      "Creates arena team",                            NULL, 0, 0, 0 },
		{ "createteam",      '9', &ChatHandler::HandleArenaCreateTeamCommand,      "Creates arena team",                            NULL, 0, 0, 0 },
		{ "setteamleader",   '8', &ChatHandler::HandleArenaSetTeamLeaderCommand,   "Sets the arena team leader",                    NULL, 0, 0, 0 },
		{ "setteamleader",   '9', &ChatHandler::HandleArenaSetTeamLeaderCommand,   "Sets the arena team leader",                    NULL, 0, 0, 0 },
		{ "resetallratings", '8', &ChatHandler::HandleArenaResetAllRatingsCommand, "Resets all arena teams to their default rating", NULL, 0, 0, 0 },
		{ "resetallratings", '9', &ChatHandler::HandleArenaResetAllRatingsCommand, "Resets all arena teams to their default rating", NULL, 0, 0, 0 },
		{ NULL,              '0', NULL,                                            "",                                              NULL, 0, 0, 0 }
	};
	dupe_command_table(arenaCommandTable, _arenaCommandTable);

	static ChatCommand achievementCommandTable[] =
	{
#ifdef ENABLE_ACHIEVEMENTS
		{ "complete", '8', &ChatHandler::HandleAchievementCompleteCommand, "Completes the specified achievement.",          NULL, 0, 0, 0 },
		{ "complete", '9', &ChatHandler::HandleAchievementCompleteCommand, "Completes the specified achievement.",          NULL, 0, 0, 0 },
		{ "criteria", '8', &ChatHandler::HandleAchievementCriteriaCommand, "Completes the specified achievement criteria.", NULL, 0, 0, 0 },
		{ "criteria", '9', &ChatHandler::HandleAchievementCriteriaCommand, "Completes the specified achievement criteria.", NULL, 0, 0, 0 },
		{ "reset",    '8', &ChatHandler::HandleAchievementResetCommand,    "Resets achievement data from the target.",      NULL, 0, 0, 0 },
		{ "reset",    '9', &ChatHandler::HandleAchievementResetCommand,    "Resets achievement data from the target.",      NULL, 0, 0, 0 },
#endif
		{ NULL,       '0', NULL,                                           "",                                              NULL, 0, 0, 0 }
	};
	dupe_command_table(achievementCommandTable, _achievementCommandTable);

	static ChatCommand vehicleCommandTable[] = {
		{ "ejectpassenger",       '8', &ChatHandler::HandleVehicleEjectPassengerCommand,     "Ejects the passenger from the specified seat",      NULL, 0, 0, 0 },
		{ "ejectpassenger",       '9', &ChatHandler::HandleVehicleEjectPassengerCommand,     "Ejects the passenger from the specified seat",      NULL, 0, 0, 0 },
		{ "ejectallpassengers",   '8', &ChatHandler::HandleVehicleEjectAllPassengersCommand, "Ejects all passengers from the vehicle",            NULL, 0, 0, 0 },
		{ "ejectallpassengers",   '9', &ChatHandler::HandleVehicleEjectAllPassengersCommand, "Ejects all passengers from the vehicle",            NULL, 0, 0, 0 },
		{ "installaccessories",   '8', &ChatHandler::HandleVehicleInstallAccessoriesCommand, "Installs the accessories for the selected vehicle", NULL, 0, 0, 0 },
		{ "installaccessories",   '9', &ChatHandler::HandleVehicleInstallAccessoriesCommand, "Installs the accessories for the selected vehicle", NULL, 0, 0, 0 },
		{ "removeaccessories",    '8', &ChatHandler::HandleVehicleRemoveAccessoriesCommand,  "Removes the accessories of the selected vehicle",   NULL, 0, 0, 0 },
		{ "removeaccessories",    '9', &ChatHandler::HandleVehicleRemoveAccessoriesCommand,  "Removes the accessories of the selected vehicle",   NULL, 0, 0, 0 },
		{ "addpassenger",         '8', &ChatHandler::HandleVehicleAddPassengerCommand,       "Adds a new NPC passenger to the vehicle",           NULL, 0, 0, 0 },
		{ "addpassenger",         '9', &ChatHandler::HandleVehicleAddPassengerCommand,       "Adds a new NPC passenger to the vehicle",           NULL, 0, 0, 0 },
		{ NULL,                   '0', NULL,                                                 "",                                                  NULL, 0, 0, 0 }
	};

	dupe_command_table(vehicleCommandTable, _vehicleCommandTable);

	static ChatCommand commandTable[] =
	{
		{ "commands",        '0', &ChatHandler::HandleCommandsCommand,                      "Shows commands",                                                                                                                          NULL,                     0, 0, 0 },
		{ "help",            '0', &ChatHandler::HandleHelpCommand,                          "Shows help for command",                                                                                                                  NULL,                     0, 0, 0 },
		{ "calcdist",        '8', &ChatHandler::HandleSimpleDistanceCommand,                "Display the distance between your current position and the specified point x y z",                                                           NULL,                     0, 0, 0 },
		{ "calcdist",        '9', &ChatHandler::HandleSimpleDistanceCommand,                "Display the distance between your current position and the specified point x y z",                                                           NULL,                     0, 0, 0 },
		{ "announce",        '2', &ChatHandler::HandleAnnounceCommand,                      "Sends a normal chat message broadcast to all players.",                                                                                                                        NULL,                     0, 0, 0 },
		{ "announce",        '5', &ChatHandler::HandleAnnounceCommand,                      "Sends a normal chat message broadcast to all players.",                                                                                                                        NULL,                     0, 0, 0 },
		{ "announce",        '8', &ChatHandler::HandleAnnounceCommand,                      "Sends a normal chat message broadcast to all players.",                                                                                                                        NULL,                     0, 0, 0 },
		{ "announce",        '9', &ChatHandler::HandleAnnounceCommand,                      "Sends a normal chat message broadcast to all players.",                                                                                                                        NULL,                     0, 0, 0 },
		{ "wannounce",       '2', &ChatHandler::HandleWAnnounceCommand,                     "Sends a widescreen raid style announcement to all players.",                                                                                                             NULL,                     0, 0, 0 },
		{ "wannounce",       '5', &ChatHandler::HandleWAnnounceCommand,                     "Sends a widescreen raid style announcement to all players.",                                                                                                             NULL,                     0, 0, 0 },
		{ "wannounce",       '8', &ChatHandler::HandleWAnnounceCommand,                     "Sends a widescreen raid style announcement to all players.",                                                                                                             NULL,                     0, 0, 0 },
		{ "wannounce",       '9', &ChatHandler::HandleWAnnounceCommand,                     "Sends a widescreen raid style announcement to all players.",                                                                                                             NULL,                     0, 0, 0 },
		{ "appear",          '2', &ChatHandler::HandleAppearCommand,                        "Teleports to x's position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "appear",          '5', &ChatHandler::HandleAppearCommand,                        "Teleports to x's position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "appear",          '8', &ChatHandler::HandleAppearCommand,                        "Teleports to x's position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "appear",          '9', &ChatHandler::HandleAppearCommand,                        "Teleports to x's position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "summon",          '2', &ChatHandler::HandleSummonCommand,                        "Summons x to your position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "summon",          '5', &ChatHandler::HandleSummonCommand,                        "Summons x to your position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "summon",          '8', &ChatHandler::HandleSummonCommand,                        "Summons x to your position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "summon",          '9', &ChatHandler::HandleSummonCommand,                        "Summons x to your position.",                                                                                                              NULL,                     0, 0, 0 },
		{ "killplr",         '9', &ChatHandler::HandleKillByPlrCommand,                     ".killplr <name> - Kills specified player",                                                                                                NULL,                     0, 0, 0 },
		{ "revive",          '2', &ChatHandler::HandleReviveCommand,                        "Revives you.",                                                                                                                            NULL,                     0, 0, 0 },
		{ "revive",          '5', &ChatHandler::HandleReviveCommand,                        "Revives you.",                                                                                                                            NULL,                     0, 0, 0 },
		{ "revive",          '8', &ChatHandler::HandleReviveCommand,                        "Revives you.",                                                                                                                            NULL,                     0, 0, 0 },
		{ "revive",          '9', &ChatHandler::HandleReviveCommand,                        "Revives you.",                                                                                                                            NULL,                     0, 0, 0 },
		{ "reviveplr",       '2', &ChatHandler::HandleReviveStringcommand,                  "Revives player specified.",                                                                                                               NULL,                     0, 0, 0 },
		{ "reviveplr",       '5', &ChatHandler::HandleReviveStringcommand,                  "Revives player specified.",                                                                                                               NULL,                     0, 0, 0 },
		{ "reviveplr",       '8', &ChatHandler::HandleReviveStringcommand,                  "Revives player specified.",                                                                                                               NULL,                     0, 0, 0 },
		{ "reviveplr",       '9', &ChatHandler::HandleReviveStringcommand,                  "Revives player specified.",                                                                                                               NULL,                     0, 0, 0 },
		{ "demorph",         '2', &ChatHandler::HandleDeMorphCommand,                       "Demorphs from morphed model.",                                                                                                            NULL,                     0, 0, 0 },
		{ "demorph",         '5', &ChatHandler::HandleDeMorphCommand,                       "Demorphs from morphed model.",                                                                                                            NULL,                     0, 0, 0 },
		{ "demorph",         '8', &ChatHandler::HandleDeMorphCommand,                       "Demorphs from morphed model.",                                                                                                            NULL,                     0, 0, 0 },
		{ "demorph",         '9', &ChatHandler::HandleDeMorphCommand,                       "Demorphs from morphed model.",                                                                                                            NULL,                     0, 0, 0 },
		{ "mount",           '8', &ChatHandler::HandleMountCommand,                         "Mounts into modelid x.",                                                                                                                  NULL,                     0, 0, 0 },
		{ "mount",           '9', &ChatHandler::HandleMountCommand,                         "Mounts into modelid x.",                                                                                                                  NULL,                     0, 0, 0 },
		{ "dismount",        '2', &ChatHandler::HandleDismountCommand,                      "Dismounts.",                                                                                                                              NULL,                     0, 0, 0 },
		{ "dismount",        '5', &ChatHandler::HandleDismountCommand,                      "Dismounts.",                                                                                                                              NULL,                     0, 0, 0 },
		{ "dismount",        '8', &ChatHandler::HandleDismountCommand,                      "Dismounts.",                                                                                                                              NULL,                     0, 0, 0 },
		{ "dismount",        '9', &ChatHandler::HandleDismountCommand,                      "Dismounts.",                                                                                                                              NULL,                     0, 0, 0 },
		{ "gps",             '5', &ChatHandler::HandleGPSCommand,                           "Shows Position",                                                                                                                          NULL,                     0, 0, 0 },
		{ "gps",             '8', &ChatHandler::HandleGPSCommand,                           "Shows Position",                                                                                                                          NULL,                     0, 0, 0 },
		{ "gps",             '9', &ChatHandler::HandleGPSCommand,                           "Shows Position",                                                                                                                          NULL,                     0, 0, 0 },
		{ "worldport",       '5', &ChatHandler::HandleWorldPortCommand,                     "Teleports you to a location with mapid x y z",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "worldport",       '8', &ChatHandler::HandleWorldPortCommand,                     "Teleports you to a location with mapid x y z",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "worldport",       '9', &ChatHandler::HandleWorldPortCommand,                     "Teleports you to a location with mapid x y z",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "start",           '0', &ChatHandler::HandleStartCommand,                         "Teleports you to a starting location",                                                                                                   NULL,                     0, 0, 0 },
		{ "devtag",			 '8', &ChatHandler::HandleDeveloperCommand,						"toggles <dev> tag",																													   NULL,					0, 0, 0 },
		{ "devtag",			 '9', &ChatHandler::HandleDeveloperCommand,						"toggles <dev> tag",																													   NULL,					0, 0, 0 },
		{ "invincible",      '8', &ChatHandler::HandleInvincibleCommand,                    ".invincible - Toggles INVINCIBILITY (mobs won't attack you)",                                                                             NULL,                     0, 0, 0 },
		{ "invincible",      '9', &ChatHandler::HandleInvincibleCommand,                    ".invincible - Toggles INVINCIBILITY (mobs won't attack you)",                                                                             NULL,                     0, 0, 0 },
		{ "invisible",       '2', &ChatHandler::HandleInvisibleCommand,                     ".invisible - Toggles INVINCIBILITY and INVISIBILITY (mobs won't attack you and nobody can see you, but they can see your chat messages)", NULL,                     0, 0, 0 },
		{ "invisible",       '5', &ChatHandler::HandleInvisibleCommand,                     ".invisible - Toggles INVINCIBILITY and INVISIBILITY (mobs won't attack you and nobody can see you, but they can see your chat messages)", NULL,                     0, 0, 0 },
		{ "invisible",       '8', &ChatHandler::HandleInvisibleCommand,                     ".invisible - Toggles INVINCIBILITY and INVISIBILITY (mobs won't attack you and nobody can see you, but they can see your chat messages)", NULL,                     0, 0, 0 },
		{ "invisible",       '9', &ChatHandler::HandleInvisibleCommand,                     ".invisible - Toggles INVINCIBILITY and INVISIBILITY (mobs won't attack you and nobody can see you, but they can see your chat messages)", NULL,                     0, 0, 0 },
		{ "playerinfo",      '2', &ChatHandler::HandlePlayerInfo,                           ".playerinfo - Displays information about the selected character (account...)",                                                           NULL,                     0, 0, 0 },
		{ "playerinfo",      '5', &ChatHandler::HandlePlayerInfo,                           ".playerinfo - Displays information about the selected character (account...)",                                                           NULL,                     0, 0, 0 },
		{ "playerinfo",      '8', &ChatHandler::HandlePlayerInfo,                           ".playerinfo - Displays information about the selected character (account...)",                                                           NULL,                     0, 0, 0 },
		{ "playerinfo",      '9', &ChatHandler::HandlePlayerInfo,                           ".playerinfo - Displays information about the selected character (account...)",                                                           NULL,                     0, 0, 0 },
		{ "levelup",         '8', &ChatHandler::HandleLevelUpCommand,                       "Levelup x lvls",                                                                                                                          NULL,                     0, 0, 0 },
		{ "levelup",         '9', &ChatHandler::HandleLevelUpCommand,                       "Levelup x lvls",                                                                                                                          NULL,                     0, 0, 0 },
		{ "modify",          '2', NULL,                                                     "",                                                                                                                                        modifyCommandTable,       0, 0, 0 },
		{ "modify",          '5', NULL,                                                     "",                                                                                                                                        modifyCommandTable,       0, 0, 0 },
		{ "modify",          '8', NULL,                                                     "",                                                                                                                                        modifyCommandTable,       0, 0, 0 },
		{ "modify",          '9', NULL,                                                     "",                                                                                                                                        modifyCommandTable,       0, 0, 0 },
		{ "waypoint",        '8', NULL,                                                     "",                                                                                                                                        waypointCommandTable,     0, 0, 0 },
		{ "waypoint",        '9', NULL,                                                     "",                                                                                                                                        waypointCommandTable,     0, 0, 0 },
		{ "debug",           '8', NULL,                                                     "",                                                                                                                                        debugCommandTable,        0, 0, 0 },
		{ "debug",           '9', NULL,                                                     "",                                                                                                                                        debugCommandTable,        0, 0, 0 },
		{ "gm",              '0', NULL,                                                     "",                                                                                                                                        gmCommandTable,           0, 0, 0 },
		{ "gm",              '2', NULL,                                                     "",                                                                                                                                        gmCommandTable,           0, 0, 0 },
		{ "gm",              '5', NULL,                                                     "",                                                                                                                                        gmCommandTable,           0, 0, 0 },
		{ "gm",              '8', NULL,                                                     "",                                                                                                                                        gmCommandTable,           0, 0, 0 },
		{ "gm",              '9', NULL,                                                     "",                                                                                                                                        gmCommandTable,           0, 0, 0 },
		{ "gmTicket",        '2', NULL,                                                     "",                                                                                                                                        GMTicketCommandTable,     0, 0, 0 },
		{ "gmTicket",        '5', NULL,                                                     "",                                                                                                                                        GMTicketCommandTable,     0, 0, 0 },
		{ "gmTicket",        '8', NULL,                                                     "",                                                                                                                                        GMTicketCommandTable,     0, 0, 0 },
		{ "gmTicket",        '9', NULL,                                                     "",                                                                                                                                        GMTicketCommandTable,     0, 0, 0 },
		{ "gobject",         '2', NULL,                                                     "",                                                                                                                                        GameObjectCommandTable,   0, 0, 0 },
		{ "gobject",         '5', NULL,                                                     "",                                                                                                                                        GameObjectCommandTable,   0, 0, 0 },
		{ "gobject",         '8', NULL,                                                     "",                                                                                                                                        GameObjectCommandTable,   0, 0, 0 },
		{ "gobject",         '9', NULL,                                                     "",                                                                                                                                        GameObjectCommandTable,   0, 0, 0 },
		{ "battleground",    '5', NULL,                                                     "",                                                                                                                                        BattlegroundCommandTable, 0, 0, 0 },
		{ "battleground",    '8', NULL,                                                     "",                                                                                                                                        BattlegroundCommandTable, 0, 0, 0 },
		{ "battleground",    '9', NULL,                                                     "",                                                                                                                                        BattlegroundCommandTable, 0, 0, 0 },
		{ "npc",             '8', NULL,                                                     "",                                                                                                                                        NPCCommandTable,          0, 0, 0 },
		{ "npc",             '9', NULL,                                                     "",                                                                                                                                        NPCCommandTable,          0, 0, 0 },
		{ "cheat",           '2', NULL,                                                     "",                                                                                                                                        CheatCommandTable,        0, 0, 0 },
		{ "cheat",           '5', NULL,                                                     "",                                                                                                                                        CheatCommandTable,        0, 0, 0 },
		{ "cheat",           '8', NULL,                                                     "",                                                                                                                                        CheatCommandTable,        0, 0, 0 },
		{ "cheat",           '9', NULL,                                                     "",                                                                                                                                        CheatCommandTable,        0, 0, 0 },
		{ "account",         '2', NULL,                                                     "",                                                                                                                                        accountCommandTable,      0, 0, 0 },
		{ "account",         '5', NULL,                                                     "",                                                                                                                                        accountCommandTable,      0, 0, 0 },
		{ "account",         '8', NULL,                                                     "",                                                                                                                                        accountCommandTable,      0, 0, 0 },
		{ "account",         '9', NULL,                                                     "",                                                                                                                                        accountCommandTable,      0, 0, 0 },
		{ "honor",           '8', NULL,                                                     "",                                                                                                                                        honorCommandTable,        0, 0, 0 },
		{ "honor",           '9', NULL,                                                     "",                                                                                                                                        honorCommandTable,        0, 0, 0 },
		{ "quest",           '8', NULL,                                                     "",                                                                                                                                        questCommandTable,        0, 0, 0 },
		{ "quest",           '9', NULL,                                                     "",                                                                                                                                        questCommandTable,        0, 0, 0 },
		{ "pet",             '8', NULL,                                                     "",                                                                                                                                        petCommandTable,          0, 0, 0 },
		{ "pet",             '9', NULL,                                                     "",                                                                                                                                        petCommandTable,          0, 0, 0 },
		{ "recall",          '2', NULL,                                                     "",                                                                                                                                        recallCommandTable,       0, 0, 0 },
		{ "recall",          '5', NULL,                                                     "",                                                                                                                                        recallCommandTable,       0, 0, 0 },
		{ "recall",          '8', NULL,                                                     "",                                                                                                                                        recallCommandTable,       0, 0, 0 },
		{ "recall",          '9', NULL,                                                     "",                                                                                                                                        recallCommandTable,       0, 0, 0 },
		{ "guild",           '0', NULL,                                                     "",                                                                                                                                        GuildCommandTable,        0, 0, 0 },
		{ "guild",           '2', NULL,                                                     "",                                                                                                                                        GuildCommandTable,        0, 0, 0 },
		{ "guild",           '5', NULL,                                                     "",                                                                                                                                        GuildCommandTable,        0, 0, 0 },
		{ "guild",           '8', NULL,                                                     "",                                                                                                                                        GuildCommandTable,        0, 0, 0 },
		{ "guild",           '9', NULL,                                                     "",                                                                                                                                        GuildCommandTable,        0, 0, 0 },
		{ "server",          '0', NULL,                                                     "",                                                                                                                                        serverCommandTable,       0, 0, 0 },
		{ "server",          '2', NULL,                                                     "",                                                                                                                                        serverCommandTable,       0, 0, 0 },
		{ "server",          '5', NULL,                                                     "",                                                                                                                                        serverCommandTable,       0, 0, 0 },
		{ "server",          '8', NULL,                                                     "",                                                                                                                                        serverCommandTable,       0, 0, 0 },
		{ "server",          '9', NULL,                                                     "",                                                                                                                                        serverCommandTable,       0, 0, 0 },
		{ "character",       '2', NULL,                                                     "",                                                                                                                                        characterCommandTable,    0, 0, 0 },
		{ "character",       '5', NULL,                                                     "",                                                                                                                                        characterCommandTable,    0, 0, 0 },
		{ "character",       '8', NULL,                                                     "",                                                                                                                                        characterCommandTable,    0, 0, 0 },
		{ "character",       '9', NULL,                                                     "",                                                                                                                                        characterCommandTable,    0, 0, 0 },
		{ "lookup",          '5', NULL,                                                     "",                                                                                                                                        lookupCommandTable,       0, 0, 0 },
		{ "lookup",          '8', NULL,                                                     "",                                                                                                                                        lookupCommandTable,       0, 0, 0 },
		{ "lookup",          '9', NULL,                                                     "",                                                                                                                                        lookupCommandTable,       0, 0, 0 },
		{ "admin",           '2', NULL,                                                     "",                                                                                                                                        adminCommandTable,        0, 0, 0 },
		{ "admin",           '5', NULL,                                                     "",                                                                                                                                        adminCommandTable,        0, 0, 0 },
		{ "admin",           '8', NULL,                                                     "",                                                                                                                                        adminCommandTable,        0, 0, 0 },
		{ "admin",           '9', NULL,                                                     "",                                                                                                                                        adminCommandTable,        0, 0, 0 },
		{ "kick",            '2', NULL,                                                     "",                                                                                                                                        kickCommandTable,         0, 0, 0 },
		{ "kick",            '5', NULL,                                                     "",                                                                                                                                        kickCommandTable,         0, 0, 0 },
		{ "kick",            '8', NULL,                                                     "",                                                                                                                                        kickCommandTable,         0, 0, 0 },
		{ "kick",            '9', NULL,                                                     "",                                                                                                                                        kickCommandTable,         0, 0, 0 },
		{ "ban",             '2', NULL,                                                     "",                                                                                                                                        banCommandTable,          0, 0, 0 },
		{ "ban",             '5', NULL,                                                     "",                                                                                                                                        banCommandTable,          0, 0, 0 },
		{ "ban",             '8', NULL,                                                     "",                                                                                                                                        banCommandTable,          0, 0, 0 },
		{ "ban",             '9', NULL,                                                     "",                                                                                                                                        banCommandTable,          0, 0, 0 },
		{ "unban",           '2', NULL,                                                     "",                                                                                                                                        unbanCommandTable,        0, 0, 0 },
		{ "unban",           '5', NULL,                                                     "",                                                                                                                                        unbanCommandTable,        0, 0, 0 },
		{ "unban",           '8', NULL,                                                     "",                                                                                                                                        unbanCommandTable,        0, 0, 0 },
		{ "unban",           '9', NULL,                                                     "",                                                                                                                                        unbanCommandTable,        0, 0, 0 },
		{ "instance",        '0', NULL,                                                     "",                                                                                                                                        instanceCommandTable,     0, 0, 0 },
		{ "instance",        '2', NULL,                                                     "",                                                                                                                                        instanceCommandTable,     0, 0, 0 },
		{ "instance",        '5', NULL,                                                     "",                                                                                                                                        instanceCommandTable,     0, 0, 0 },
		{ "instance",        '8', NULL,                                                     "",                                                                                                                                        instanceCommandTable,     0, 0, 0 },
		{ "instance",        '9', NULL,                                                     "",                                                                                                                                        instanceCommandTable,     0, 0, 0 },
		{ "arena",           '8', NULL,                                                     "",                                                                                                                                        arenaCommandTable,        0, 0, 0 },
		{ "arena",           '9', NULL,                                                     "",                                                                                                                                        arenaCommandTable,        0, 0, 0 },
		{ "kp",      '2', &ChatHandler::HandleKickCommand,                          "Kicks player from server",                                                                                                                NULL,                     0, 0, 0 },
		{ "kp",      '5', &ChatHandler::HandleKickCommand,                          "Kicks player from server",                                                                                                                NULL,                     0, 0, 0 },
		{ "kp",      '8', &ChatHandler::HandleKickCommand,                          "Kicks player from server",                                                                                                                NULL,                     0, 0, 0 },
		{ "kp",      '9', &ChatHandler::HandleKickCommand,                          "Kicks player from server",                                                                                                                NULL,                     0, 0, 0 },
		{ "gmannounce",      '2', &ChatHandler::HandleGMAnnounceCommand,                    "Sends Msg to all online GMs",                                                                                                             NULL,                     0, 0, 0 },
		{ "gmannounce",      '5', &ChatHandler::HandleGMAnnounceCommand,                    "Sends Msg to all online GMs",                                                                                                             NULL,                     0, 0, 0 },
		{ "gmannounce",      '8', &ChatHandler::HandleGMAnnounceCommand,                    "Sends Msg to all online GMs",                                                                                                             NULL,                     0, 0, 0 },
		{ "gmannounce",      '9', &ChatHandler::HandleGMAnnounceCommand,                    "Sends Msg to all online GMs",                                                                                                             NULL,                     0, 0, 0 },
		{ "clearcooldowns",  '8', &ChatHandler::HandleClearCooldownsCommand,                "Clears all cooldowns for your class.",                                                                                                    NULL,                     0, 0, 0 },
		{ "clearcooldowns",  '9', &ChatHandler::HandleClearCooldownsCommand,                "Clears all cooldowns for your class.",                                                                                                    NULL,                     0, 0, 0 },
		{ "removeauras",     '2', &ChatHandler::HandleRemoveAurasCommand,                   "Removes all auras from target",                                                                                                           NULL,                     0, 0, 0 },
		{ "removeauras",     '5', &ChatHandler::HandleRemoveAurasCommand,                   "Removes all auras from target",                                                                                                           NULL,                     0, 0, 0 },
		{ "removeauras",     '8', &ChatHandler::HandleRemoveAurasCommand,                   "Removes all auras from target",                                                                                                           NULL,                     0, 0, 0 },
		{ "removeauras",     '9', &ChatHandler::HandleRemoveAurasCommand,                   "Removes all auras from target",                                                                                                           NULL,                     0, 0, 0 },
		{ "paralyze",        '2', &ChatHandler::HandleParalyzeCommand,                      "Roots/Paralyzes the target.",                                                                                                             NULL,                     0, 0, 0 },
		{ "paralyze",        '5', &ChatHandler::HandleParalyzeCommand,                      "Roots/Paralyzes the target.",                                                                                                             NULL,                     0, 0, 0 },
		{ "paralyze",        '8', &ChatHandler::HandleParalyzeCommand,                      "Roots/Paralyzes the target.",                                                                                                             NULL,                     0, 0, 0 },
		{ "paralyze",        '9', &ChatHandler::HandleParalyzeCommand,                      "Roots/Paralyzes the target.",                                                                                                             NULL,                     0, 0, 0 },
		{ "unparalyze",      '2', &ChatHandler::HandleUnParalyzeCommand,                    "Unroots/Unparalyzes the target.",                                                                                                         NULL,                     0, 0, 0 },
		{ "unparalyze",      '5', &ChatHandler::HandleUnParalyzeCommand,                    "Unroots/Unparalyzes the target.",                                                                                                         NULL,                     0, 0, 0 },
		{ "unparalyze",      '8', &ChatHandler::HandleUnParalyzeCommand,                    "Unroots/Unparalyzes the target.",                                                                                                         NULL,                     0, 0, 0 },
		{ "unparalyze",      '9', &ChatHandler::HandleUnParalyzeCommand,                    "Unroots/Unparalyzes the target.",                                                                                                         NULL,                     0, 0, 0 },
		{ "gotrig",          '9', &ChatHandler::HandleTriggerCommand,                       "Warps to areatrigger <id>",                                                                                                               NULL,                     0, 0, 0 },
		{ "modperiod",       '8', &ChatHandler::HandleModPeriodCommand,                     "Changes period of current transporter.",                                                                                                  NULL,                     0, 0, 0 },
		{ "modperiod",       '9', &ChatHandler::HandleModPeriodCommand,                     "Changes period of current transporter.",                                                                                                  NULL,                     0, 0, 0 },
		{ "logcomment",      '2', &ChatHandler::HandleGmLogCommentCommand,                  "Adds a comment to the 2 log for the admins/devs to read.",                                                                                    NULL,                     0, 0, 0 },
		{ "logcomment",      '5', &ChatHandler::HandleGmLogCommentCommand,                  "Adds a comment to the 2 log for the admins/devs to read.",                                                                                    NULL,                     0, 0, 0 },
		{ "logcomment",      '8', &ChatHandler::HandleGmLogCommentCommand,                  "Adds a comment to the 2 log for the admins/devs to read.",                                                                                    NULL,                     0, 0, 0 },
		{ "logcomment",      '9', &ChatHandler::HandleGmLogCommentCommand,                  "Adds a comment to the 2 log for the admins/devs to read.",                                                                                    NULL,                     0, 0, 0 },
		{ "removesickness",  '2', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand, "Removes ressurrection sickness from the target",                                                                                          NULL,                     0, 0, 0 },
		{ "removesickness",  '5', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand, "Removes ressurrection sickness from the target",                                                                                          NULL,                     0, 0, 0 },
		{ "removesickness",  '8', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand, "Removes ressurrection sickness from the target",                                                                                          NULL,                     0, 0, 0 },
		{ "removesickness",  '9', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand, "Removes ressurrection sickness from the target",                                                                                          NULL,                     0, 0, 0 },
		{ "fixscale",        '8', &ChatHandler::HandleFixScaleCommand,                      "",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "fixscale",        '9', &ChatHandler::HandleFixScaleCommand,                      "",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "addtrainerspell", '8', &ChatHandler::HandleAddTrainerSpellCommand,               "",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "addtrainerspell", '9', &ChatHandler::HandleAddTrainerSpellCommand,               "",                                                                                                                                        NULL,                     0, 0, 0 },
		{ "achieve",         '9', NULL,                                                     "",                                                                                                                                        achievementCommandTable,  0, 0, 0 },
		{ "vehicle",         '8', NULL,                                                     "",                                                                                                                                        vehicleCommandTable,      0, 0, 0 },
		{ "vehicle",         '9', NULL,                                                     "",                                                                                                                                        vehicleCommandTable,      0, 0, 0 },
		{ "additem",             '9', &ChatHandler::HandleAddInvItemCommand,       "Adds item x count y",                                                                                                                  NULL, 0, 0, 0 },
		{ "removeitem",          '9', &ChatHandler::HandleRemoveItemCommand,       "Removes item x count y.",                                                                                         NULL, 0, 0, 0 },
		{ NULL,              '0', NULL,                                                     "",                                                                                                                                        NULL,                     0, 0, 0 }
	};
	dupe_command_table(commandTable, _commandTable);

	/* set the correct pointers */
	ChatCommand* p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(p->ChildCommands != 0)
		{
			// Set the correct pointer.
			ChatCommand* np = GetSubCommandTable(p->Name);
			ARCEMU_ASSERT(np != NULL);
			p->ChildCommands = np;
		}
		++p;
	}
}

ChatHandler::ChatHandler()
{
	new CommandTableStorage;
	CommandTableStorage::getSingleton().Init();
	SkillNameManager = new SkillNameMgr;
}

ChatHandler::~ChatHandler()
{
	CommandTableStorage::getSingleton().Dealloc();
	delete CommandTableStorage::getSingletonPtr();
	delete SkillNameManager;
}

bool ChatHandler::hasStringAbbr(const char* s1, const char* s2)
{
	for(;;)
	{
		if(!*s2)
			return true;
		else if(!*s1)
			return false;
		else if(tolower(*s1) != tolower(*s2))
			return false;
		s1++;
		s2++;
	}
}

void ChatHandler::SendMultilineMessage(WorldSession* m_session, const char* str)
{
	char* start = (char*)str, *end;
	for(;;)
	{
		end = strchr(start, '\n');
		if(!end)
			break;

		*end = '\0';
		SystemMessage(m_session, start);
		start = end + 1;
	}
	if(*start != '\0')
		SystemMessage(m_session, start);
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand* table, const char* text, WorldSession* m_session)
{
	std::string cmd = "";

	// get command
	while(*text != ' ' && *text != '\0')
	{
		cmd += *text;
		text++;
	}

	while(*text == ' ') text++;  // skip whitespace

	if(!cmd.length())
		return false;

	for(uint32 i = 0; table[i].Name != NULL; i++)
	{
		if(!hasStringAbbr(table[i].Name, cmd.c_str()))
			continue;

		if(table[i].CommandGroup != '0' && !m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		if(table[i].ChildCommands != NULL)
		{
			if(!ExecuteCommandInTable(table[i].ChildCommands, text, m_session))
			{
				if(table[i].Help != "")
					SendMultilineMessage(m_session, table[i].Help.c_str());
				else
				{
					GreenSystemMessage(m_session, "Available Subcommands:");
					for(uint32 k = 0; table[i].ChildCommands[k].Name; k++)
					{
						if(table[i].ChildCommands[k].CommandGroup == '0' || (table[i].ChildCommands[k].CommandGroup != '0' && m_session->CanUseCommand(table[i].ChildCommands[k].CommandGroup)))
							BlueSystemMessage(m_session, " %s - %s", table[i].ChildCommands[k].Name, table[i].ChildCommands[k].Help.size() ? table[i].ChildCommands[k].Help.c_str() : "No Help Available");
					}
				}
			}

			return true;
		}

		// Check for field-based commands
		if(table[i].Handler == NULL && (table[i].MaxValueField || table[i].NormalValueField))
		{
			bool result = false;
			if(strlen(text) == 0)
			{
				RedSystemMessage(m_session, "No values specified.");
			}
			if(table[i].ValueType == 2)
				result = CmdSetFloatField(m_session, table[i].NormalValueField, table[i].MaxValueField, table[i].Name, text);
			else
				result = CmdSetValueField(m_session, table[i].NormalValueField, table[i].MaxValueField, table[i].Name, text);
			if(!result)
				RedSystemMessage(m_session, "Must be in the form of (command) <value>, or, (command) <value> <maxvalue>");
		}
		else
		{
			if(!(this->*(table[i].Handler))(text, m_session))
			{
				if(table[i].Help != "")
					SendMultilineMessage(m_session, table[i].Help.c_str());
				else
				{
					RedSystemMessage(m_session, "Incorrect syntax specified. Try .help %s for the correct syntax.", table[i].Name);
				}
			}
		}

		return true;
	}
	return false;
}

int ChatHandler::ParseCommands(const char* text, WorldSession* session)
{
	if(!session)
		return 0;

	if(!*text)
		return 0;

	if(session->GetPermissionCount() == 0 && sWorld.m_reqGmForCommands)
		return 0;

	if(text[0] != '!' && text[0] != '.') // let's not confuse users
		return 0;

	/* skip '..' :P that pisses me off */
	if(text[1] == '.')
		return 0;

	text++;

	if(!ExecuteCommandInTable(CommandTableStorage::getSingleton().Get(), text, session))
	{
		SystemMessage(session, "There is no such command, or you do not have access to it.");
	}

	return 1;
}

WorldPacket* ChatHandler::FillMessageData(uint32 type, uint32 language, const char* message, uint64 guid , uint8 flag) const
{
	//Packet    structure
	//uint8	    type;
	//uint32	language;
	//uint64	guid;
	//uint64	guid;
	//uint32	len_of_text;
	//char	    text[];		 // not sure ? i think is null terminated .. not null terminated
	//uint8	    afk_state;
	ARCEMU_ASSERT(type != CHAT_MSG_CHANNEL);
	//channels are handled in channel handler and so on
	uint32 messageLength = (uint32)strlen(message) + 1;

	WorldPacket* data = new WorldPacket(SMSG_MESSAGECHAT, messageLength + 30);

	*data << (uint8)type;
	*data << language;

	*data << guid;
	*data << uint32(0);

	*data << guid;

	*data << messageLength;
	*data << message;

	*data << uint8(flag);
	return data;
}

WorldPacket* ChatHandler::FillSystemMessageData(const char* message) const
{
	uint32 messageLength = (uint32)strlen(message) + 1;

	WorldPacket* data = new WorldPacket(SMSG_MESSAGECHAT, 30 + messageLength);
	*data << (uint8)CHAT_MSG_SYSTEM;
	*data << (uint32)LANG_UNIVERSAL;

	// Who cares about guid when there's no nickname displayed heh ?
	*data << (uint64)0;
	*data << (uint32)0;
	*data << (uint64)0;

	*data << messageLength;
	*data << message;

	*data << uint8(0);

	return data;
}

Player* ChatHandler::getSelectedChar(WorldSession* m_session, bool showerror)
{
	uint64 guid;
	Player* chr;

	if(m_session == NULL || m_session->GetPlayer() == NULL) return NULL;

	guid = m_session->GetPlayer()->GetSelection();

	if(guid == 0)
	{
		if(showerror)
			GreenSystemMessage(m_session, "Auto-targeting self.");
		chr = m_session->GetPlayer(); // autoselect
	}
	else
		chr = m_session->GetPlayer()->GetMapMgr()->GetPlayer((uint32)guid);

	if(chr == NULL)
	{
		if(showerror)
			RedSystemMessage(m_session, "This command requires that you select a player.");
		return NULL;
	}

	return chr;
}

Creature* ChatHandler::getSelectedCreature(WorldSession* m_session, bool showerror)
{
	uint64 guid;
	Creature* creature = NULL;

	if(m_session == NULL || m_session->GetPlayer() == NULL) return NULL;

	guid = m_session->GetPlayer()->GetSelection();

	switch( GET_TYPE_FROM_GUID( guid ) ){
		case HIGHGUID_TYPE_PET:
			creature = m_session->GetPlayer()->GetMapMgr()->GetPet(GET_LOWGUID_PART(guid));
			break;

		case HIGHGUID_TYPE_UNIT:
		case HIGHGUID_TYPE_VEHICLE:
			creature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
			break;
	}

	if(creature != NULL)
		return creature;
	else
	{
		if(showerror)
			RedSystemMessage(m_session, "This command requires that you select a creature.");
		return NULL;
	}
}

void ChatHandler::SystemMessage(WorldSession* m_session, const char* message, ...)
{
	if(!message) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	WorldPacket* data = FillSystemMessageData(msg1);
	if(m_session != NULL)
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::ColorSystemMessage(WorldSession* m_session, const char* colorcode, const char* message, ...)
{
	if(!message) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", colorcode, msg1);
	WorldPacket* data = FillSystemMessageData(msg);
	if(m_session != NULL)
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessage(WorldSession* m_session, const char* message, ...)
{
	if(!message) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", MSG_COLOR_LIGHTRED/*MSG_COLOR_RED*/, msg1);
	WorldPacket* data = FillSystemMessageData(msg);
	if(m_session != NULL)
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::GreenSystemMessage(WorldSession* m_session, const char* message, ...)
{
	if(!message) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", MSG_COLOR_GREEN, msg1);
	WorldPacket* data = FillSystemMessageData(msg);
	if(m_session != NULL)
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::BlueSystemMessage(WorldSession* m_session, const char* message, ...)
{
	if(!message) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", MSG_COLOR_LIGHTBLUE, msg1);
	WorldPacket* data = FillSystemMessageData(msg);
	if(m_session != NULL)
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessageToPlr(Player* plr, const char* message, ...)
{
	if(!message || !plr || !plr->GetSession()) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	RedSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::GreenSystemMessageToPlr(Player* plr, const char* message, ...)
{
	if(!message || !plr || !plr->GetSession()) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	GreenSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::BlueSystemMessageToPlr(Player* plr, const char* message, ...)
{
	if(!message || !plr || !plr->GetSession()) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	BlueSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::SystemMessageToPlr(Player* plr, const char* message, ...)
{
	if(!message || !plr || !plr->GetSession()) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1, 1024, message, ap);
	SystemMessage(plr->GetSession(), msg1);
}

bool ChatHandler::CmdSetValueField(WorldSession* m_session, uint32 field, uint32 fieldmax, const char* fieldname, const char* args)
{
	char* pvalue;
	uint32 mv, av;

	if(!args || !m_session) return false;

	pvalue = strtok((char*)args, " ");
	if(!pvalue)
		return false;
	else
		av = atol(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok(NULL, " ");
		if(!pvaluemax)
			return false;
		else
			mv = atol(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if(av <= 0 && mv > 0)
	{
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;
		}
	}

	Player* plr = getSelectedChar(m_session, false);
	if(plr)
	{
		sGMLog.writefromsession(m_session, "used modify field value: %s, %u on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d/%d.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d/%d.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d.", m_session->GetPlayer()->GetName(), fieldname, av);
		}

		if(field == UNIT_FIELD_STAT1) av /= 2;
		if(field == UNIT_FIELD_BASE_HEALTH)
		{
			plr->SetHealth(av);
		}

		plr->SetUInt32Value(field, av);

		if(fieldmax)
		{
			plr->SetUInt32Value(fieldmax, mv);
		}
	}
	else
	{
		Creature* cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			creaturename = cr->GetCreatureInfo()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %d/%d.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %d.", fieldname, creaturename.c_str(), av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %u on %s", fieldname, av, creaturename.c_str());
			if(field == UNIT_FIELD_STAT1) av /= 2;
			if(field == UNIT_FIELD_BASE_HEALTH)
				cr->SetHealth(av);

			switch(field)
			{
				case UNIT_FIELD_FACTIONTEMPLATE:
					{
						if(cr->m_spawn)
							WorldDatabase.Execute("UPDATE creature_spawns SET faction = %u WHERE entry = %u", av, cr->m_spawn->entry);
					}
					break;
				case UNIT_NPC_FLAGS:
					{
						WorldDatabase.Execute("UPDATE creature_proto SET npcflags = %u WHERE entry = %u", av, cr->GetProto()->Id);
					}
					break;
			}

			cr->SetUInt32Value(field, av);

			if(fieldmax)
			{
				cr->SetUInt32Value(fieldmax, mv);
			}
			// reset faction
			if(field == UNIT_FIELD_FACTIONTEMPLATE)
				cr->_setFaction();

			// Only actually save the change if we are modifying a spawn
			if( cr->GetSQL_id() != 0 )
				cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::CmdSetFloatField(WorldSession* m_session, uint32 field, uint32 fieldmax, const char* fieldname, const char* args)
{
	char* pvalue;
	float mv, av;

	if(!args || !m_session) return false;

	pvalue = strtok((char*)args, " ");
	if(!pvalue)
		return false;
	else
		av = (float)atof(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok(NULL, " ");
		if(!pvaluemax)
			return false;
		else
			mv = (float)atof(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if(av <= 0)
	{
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;
		}
	}

	Player* plr = getSelectedChar(m_session, false);
	if(plr)
	{
		sGMLog.writefromsession(m_session, "used modify field value: %s, %f on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f/%.1f.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f/%.1f.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f.", m_session->GetPlayer()->GetName(), fieldname, av);
		}
		plr->SetFloatValue(field, av);
		if(fieldmax) plr->SetFloatValue(fieldmax, mv);
	}
	else
	{
		Creature* cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			creaturename = cr->GetCreatureInfo()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f/%.1f.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f.", fieldname, creaturename.c_str(), av);
			cr->SetFloatValue(field, av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %f on %s", fieldname, av, creaturename.c_str());
			if(fieldmax)
			{
				cr->SetFloatValue(fieldmax, mv);
			}
			//cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::HandleGetPosCommand(const char* args, WorldSession* m_session)
{
	if(!args || !m_session) return false;

	/*if(m_session->GetPlayer()->GetSelection() == 0) return false;
	Creature *creature = objmgr.GetCreature(m_session->GetPlayer()->GetSelection());

	if(!creature) return false;
	BlueSystemMessage(m_session, "Creature Position: \nX: %f\nY: %f\nZ: %f\n", creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ());
	return true;*/

	uint32 spell = atol(args);
	SpellEntry* se = dbcSpell.LookupEntryForced(spell);
	if(se)
		BlueSystemMessage(m_session, "SpellIcon for %d is %d", se->Id, se->field114);
	return true;
}
