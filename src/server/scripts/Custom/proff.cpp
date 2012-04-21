#include "ScriptPCH.h"

class Proff_NPC : public CreatureScript
{
        public:
                Proff_NPC() : CreatureScript("Proff_NPC"){}
     
                bool OnGossipHello(Player * pPlayer, Creature * pCreature)
                {
                        pPlayer->ADD_GOSSIP_ITEM(4, "Jewelcrafting", GOSSIP_SENDER_MAIN, 1);
                        pPlayer->ADD_GOSSIP_ITEM(4, "Enchanting", GOSSIP_SENDER_MAIN, 2);
                        pPlayer->ADD_GOSSIP_ITEM(4, "Inscription", GOSSIP_SENDER_MAIN, 3);     
						pPlayer->ADD_GOSSIP_ITEM(4, "Blacksmithing", GOSSIP_SENDER_MAIN, 4); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Leatherworking", GOSSIP_SENDER_MAIN, 5); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Alchemy", GOSSIP_SENDER_MAIN, 6); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Tailoring", GOSSIP_SENDER_MAIN, 7); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Herbalism", GOSSIP_SENDER_MAIN, 8); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Skinning", GOSSIP_SENDER_MAIN, 9); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Mining", GOSSIP_SENDER_MAIN, 10); 
						pPlayer->ADD_GOSSIP_ITEM(4, "First Aid", GOSSIP_SENDER_MAIN, 11); 
						pPlayer->ADD_GOSSIP_ITEM(4, "Cooking", GOSSIP_SENDER_MAIN, 12); 
                        pPlayer->PlayerTalkClass->SendGossipMenu(9425, pCreature->GetGUID());
                        return true;
                }
     
                bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
                {
					pPlayer->PlayerTalkClass->ClearMenus();

                        switch(uiAction)
                        {
                                case 1: // JC
									{
										pPlayer->learnSpell(51311, true); //Grand Master JC
										pPlayer->UpdateSkillPro(51311, 100, 100);
									}
                                        break;


						}
                        return true;
                }
};
     
void AddSC_NPC_Proff()
{
        new Proff_NPC();
}