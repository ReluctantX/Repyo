#include "ScriptPCH.h"
     
enum defines
{
        // Token's
     
        D_TOKEN = 56808,
        V_TOKEN = 56807,
     
        // Name Change
     
        NAME_CHANGE_TOKENS_REQUIRED_D = 5,
        NAME_CHANGE_TOKENS_REQUIRED_V = 15,
     
        // Race Change
     
        RACE_CHANGE_TOKENS_REQUIRED_D = 10,
        RACE_CHANGE_TOKENS_REQUIRED_V = 30,
     
        // Faction Change
     
        FACTION_CHANGE_TOKENS_REQUIRED_D = 20,
        FACTION_CHANGE_TOKENS_REQUIRED_V = 100,
     
};
     
class Gossip_Changer : public CreatureScript
{
        public:
                Gossip_Changer() : CreatureScript("Gossip_Changer"){}
     
                bool OnGossipHello(Player * pPlayer, Creature * pCreature)
                {
                        pPlayer->ADD_GOSSIP_ITEM(4, "I want to Change my name", GOSSIP_SENDER_MAIN, 1);
                        pPlayer->ADD_GOSSIP_ITEM(4, "I want to change my race", GOSSIP_SENDER_MAIN, 4);
                        pPlayer->ADD_GOSSIP_ITEM(4, "I want to change my faction", GOSSIP_SENDER_MAIN, 7);     
                        pPlayer->PlayerTalkClass->SendGossipMenu(9425, pCreature->GetGUID());
                        return true;
                }
     
                bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
                {
					pPlayer->PlayerTalkClass->ClearMenus();

                        switch(uiAction)
                        {
                                case 1:
									{
                                        pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Donation Coin's (5) ", GOSSIP_SENDER_MAIN, 2);
                                        pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Vote Coin's (15)", GOSSIP_SENDER_MAIN, 3);
                                        pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetGUID());
									}
                                        break;
     
                                case 2:
									{
                                        if(pPlayer->HasItemCount(D_TOKEN, NAME_CHANGE_TOKENS_REQUIRED_D))
                                        {
                                                pPlayer->DestroyItemCount(D_TOKEN, NAME_CHANGE_TOKENS_REQUIRED_D, true, false);
                                                pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
                                                CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '1' WHERE guid = %u", pPlayer->GetGUIDLow());
												pPlayer->GetSession()->SendNotification("Removed x5 [Donation Coin] | Sucess, Please logout to initiate the Name change...");
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
     
                                        }
                                        else
                                        {
                                                pPlayer->GetSession()->SendNotification("Failed : You don't have the required Donation Coins! Please purchase more via the Donation shop and try again.");
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
                                        }
									}break;
     
                                case 3:
									{
										if(pPlayer->HasItemCount(V_TOKEN, NAME_CHANGE_TOKENS_REQUIRED_V))
										{
												pPlayer->DestroyItemCount(V_TOKEN, NAME_CHANGE_TOKENS_REQUIRED_V, true, false);
												pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
												CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '1' WHERE guid = %u", pPlayer->GetGUIDLow());
												pPlayer->GetSession()->SendNotification("Removed x15 [Vote Coin] | Sucess, Please logout to initiate the Name change...");
												pPlayer->PlayerTalkClass->SendCloseGossip();
     
										}
										else
										{
												pPlayer->GetSession()->SendNotification("Failed : You don't have the required Vote Coins! Please vote and purchase the coins via the vote store and try again.");
												pPlayer->PlayerTalkClass->SendCloseGossip();
										}
									}break;
     
                                case 4:
									{
                                pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Donation Coin's (10) ", GOSSIP_SENDER_MAIN, 5);
                                pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Vote Coin's (30)", GOSSIP_SENDER_MAIN, 6);
                                pPlayer->PlayerTalkClass->SendGossipMenu(1, pCreature->GetGUID());
									}
                                break;
     
                                case 5:
									{
                                        if(pPlayer->HasItemCount(D_TOKEN, RACE_CHANGE_TOKENS_REQUIRED_D))
                                        {
                                                pPlayer->DestroyItemCount(D_TOKEN, RACE_CHANGE_TOKENS_REQUIRED_D, true, false);
                                                pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                                                CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '128' WHERE guid = %u", pPlayer->GetGUIDLow());
                                                pPlayer->MonsterWhisper("Removed x10 [Donation Coin] | Sucess, Please logout to initiate the Race change...", pPlayer->GetGUID());
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
     
                                        }
                                        else
                                        {
                                                pPlayer->GetSession()->SendNotification("Failed : You don't have the required Donation Coins! Please purchase more via the Donation shop and try again.");
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
                                        }
									}break;
     
                                case 6:
									{
										if(pPlayer->HasItemCount(V_TOKEN, RACE_CHANGE_TOKENS_REQUIRED_V))
										{
												pPlayer->DestroyItemCount(V_TOKEN, RACE_CHANGE_TOKENS_REQUIRED_V, true, false);
												pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
												CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '128' WHERE guid = %u", pPlayer->GetGUIDLow());
												pPlayer->GetSession()->SendNotification("Removed x30 [Vote Coin] | Sucess, Please logout to initiate the Race change....");
												pPlayer->PlayerTalkClass->SendCloseGossip();
     
										}
										else
										{
												pPlayer->GetSession()->SendNotification("Failed : You don't have the required Vote Coins! Please vote and purchase the coins via the vote store and try again.");
												pPlayer->PlayerTalkClass->SendCloseGossip();
										}
									}break;
                                        
     
                                case 7:
                                pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Donation Coin's (20) ", GOSSIP_SENDER_MAIN, 5);
                                pPlayer->ADD_GOSSIP_ITEM(4, "Buy with Vote Coin's (100)", GOSSIP_SENDER_MAIN, 6);
                                pPlayer->PlayerTalkClass->SendGossipMenu(1, pCreature->GetGUID());
                                break;
     
                                case 8:
									{
                                        if(pPlayer->HasItemCount(D_TOKEN, FACTION_CHANGE_TOKENS_REQUIRED_D))
                                        {
                                                pPlayer->DestroyItemCount(D_TOKEN, FACTION_CHANGE_TOKENS_REQUIRED_D, true, false);
                                                pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                                                CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", pPlayer->GetGUIDLow());
                                                pPlayer->GetSession()->SendNotification("Removed x20 [Donation Coin] | Sucess, Please logout to initiate the Faction change...");
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
     
                                        }
                                        else
                                        {
                                                pPlayer->GetSession()->SendNotification("Failed : You don't have the required Donation Coins! Please purchase more via the Donation shop and try again.");
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
                                        }
									}break;
     
                                    case 9:
										{
											if(pPlayer->HasItemCount(V_TOKEN, FACTION_CHANGE_TOKENS_REQUIRED_V))
											{
													pPlayer->DestroyItemCount(V_TOKEN, FACTION_CHANGE_TOKENS_REQUIRED_V, true, false);
													pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
													CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", pPlayer->GetGUIDLow());
													pPlayer->GetSession()->SendNotification("Removed x100 [Vote Coin] | Sucess, Please logout to initiate the Faction change....");
													pPlayer->PlayerTalkClass->SendCloseGossip();
     
											}
											else
											{
													pPlayer->GetSession()->SendNotification("Failed : You don't have the required Vote Coins! Please vote and purchase the coins via the vote store and try again.");
													pPlayer->PlayerTalkClass->SendCloseGossip();
											}
										}break;
                        }
                        return true;
                }
};
     
void AddSC_NPC_Gossip_Changer()
{
        new Gossip_Changer();
}