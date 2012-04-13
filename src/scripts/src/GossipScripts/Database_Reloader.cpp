#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)
#endif

#define ITEMID 86974

class Reloader_Item : public Arcemu::Gossip::Script
{
   public:
	   void OnHello(Object * pObject, Player * Plr)
	   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Items Table", 1);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Creature Tables", 2);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Gameobject_names Table", 3);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Areatriggers Table", 4);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Itempages Table", 5);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Worldstring_tables", 6);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Worldbroadcast Table", 7);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Quests Table", 8);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Npc_Text Table", 9);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Fishing Table", 10);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Teleport_Coords Table", 11);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Graveyards Table", 12);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Worldmap_Info Table", 13);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Zoneguards Table", 14);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Unit_display_size Table", 15);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Reload Vendors Table", 16);
		   menu.Send(Plr);
	   }

	   void OnSelectOption(Object * pObject, Player * Plr, uint32 Id, const char * code)
	   {
		   GossipMenu * Menu;
		   switch(Id)
		   {
			  case 1: // Items
					{
					    char itemmsg[200];
						snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''items''. Lag may occur during this reload.|r");
						sWorld.SendWorldText(itemmsg, 0);
						ItemPrototypeStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
					}break;

					case 2: // Creature Tables
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''creature_names''. Lag may occur during this reload.|r");
                         sWorld.SendWorldText(itemmsg, 0);
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''creature_proto''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						CreatureProtoStorage.Reload();
						CreatureNameStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;
                    /*
					 case 3: // Creature_names
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''creature_names''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						CreatureNameStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;*/

					 case 4: // gameobject_names
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''gameobject_names''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						GameObjectNameStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 5: // Areatriggers
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''areatriggers''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						AreaTriggerStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 6: // itempages
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''itempages''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						ItemPageStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 7: // Worldstring_tables
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''worldstring_tables''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						WorldStringTableStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 8: // worldbroadcast
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''worldbroadcast''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						WorldBroadCastStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 9: // quests
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''quests''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						QuestStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 10: // npc_text
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''npc_text''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						NpcTextStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 11: // fishing
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''fishing''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						FishingZoneStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 12: // teleport_coords
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''teleport_coords''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						TeleportCoordStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 13: // graveyards
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''graveyards''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						GraveyardStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 14: // worldmap_info
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''worldmap_info''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						WorldMapInfoStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 15: // zoneguards
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''zoneguards''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						ZoneGuardStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 16: // unit_display_sizes
					 {
						 char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''unit_display_sizes''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						UnitModelSizeStorage.Reload();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;

					 case 17: // Vendors
						 {
						  char itemmsg[200];
						 snprintf(itemmsg, 200, "|cff6495EDRefreshing the cache of table ''Vendors''. Lag may occur during this reload.|r");
                        sWorld.SendWorldText(itemmsg, 0);
						objmgr.ReloadVendors();
						Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
						}break;
					}
				}
			};

void SetupReloader(ScriptMgr * mgr)
{
	mgr->register_item_gossip(ITEMID, new Reloader_Item);
}