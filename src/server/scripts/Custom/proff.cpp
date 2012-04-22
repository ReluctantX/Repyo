#include "ScriptPCH.h"

/*unit32 jcSkills[] =
{
	25278,
	25255,
	25280,
	25283,
	26909,
	26908,
	26907,
	26906,
	26903,
	26902,
	26900,
	26897,
	26896,
	26887,
	26885,
	26883,
	26882,
	26881,
	26880,
	26876,
	26874,
	26873,
	26872,
	25622,
	25621,
	25620,
	25619,
	25618,
	25617,
	25615,
	25614,
	25613,
	25612,
	25610,
	25498,
	25490,
	25493,
	25323,
	25321,
	25320,
	25339,
	25318,
	25317,
	25305,
	25287,
	25284,
	31113,
	31104,
	31099,
	31109,
	31099,
	31091,
	31106,
	31110,
	31085,
	31112,
	31096,
	31089,
	28957,
	28955,
	28953,
	28950,
	28948,
	28947,
};*/


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

										/*pPlayer->learnSpell(51311, true); //Grand Master JC
										for(uint32 i = 0; i < jcSkills; i++)
											{
												pPlayer->learnSpell(i, true);
											}*/

										pPlayer->learnSpell(51311, true); //Grand Master JC
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