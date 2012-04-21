#include "ScriptPCH.h"

class BeastMaster : public CreatureScript
{
	public:
		BeastMaster() : CreatureScript("BeastMaster"){}
		
		void CreatePet(Player * pPlayer, Creature * pCreature, uint32 entryid)
		{
			WorldSession * m_session = pPlayer->GetSession();
			if(pPlayer->getClass() != CLASS_HUNTER)
			{
				m_session->SendNotification("You must be a hunter to ");
				pPlayer->PlayerTalkClass->SendCloseGossip();
				return;
			}
			else if(pPlayer->GetPet())
			{
				m_session->SendNotification("You already have a pet, please stable your current one.");
				pPlayer->PlayerTalkClass->SendCloseGossip();
				return;
			}
			else
			{
				Creature *creatureTarget = pCreature->SummonCreature(entryid, pPlayer->GetPositionX(), pPlayer->GetPositionY()+2, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
				
				if(!creatureTarget)
					return;
				
				Pet* pPet = pPlayer->CreateTamedPetFrom(creatureTarget, 0);
				
				if(!pPet) 
					return;
				
				creatureTarget->setDeathState(JUST_DIED);
				creatureTarget->RemoveCorpse();
				creatureTarget->SetHealth(0);
 
				pPet->SetPower(POWER_HAPPINESS, 1048000);
				pPet->SetUInt32Value(UNIT_FIELD_LEVEL, pPlayer->getLevel() - 1);
				pPet->GetMap()->AddToMap((Creature*)pPet);
				pPet->SetUInt32Value(UNIT_FIELD_LEVEL, pPlayer->getLevel());
				
				if(!pPet->InitStatsForLevel(pPlayer->getLevel()))
					sLog->outError ("Creation of pet failed: no init stats for entry %u", entryid);
 
				pPet->UpdateAllStats();
        
				pPlayer->SetMinion(pPet, true);
 
				pPet->SavePetToDB(PET_SAVE_AS_CURRENT);
				pPet->InitTalentForLevel();
				pPlayer->PetSpellInitialize();
				pPlayer->PlayerTalkClass->SendCloseGossip();
				pCreature->MonsterWhisper("Enjoy your new pet!", pPlayer->GetGUID());
			}
		}

		bool OnGossipHello(Player * pPlayer, Creature * pCreature)
		{
			WorldSession * m_session = pPlayer->GetSession();
			if(pPlayer->getClass() != CLASS_HUNTER)
			{
				m_session->SendNotification("You aren't a hunter!");
				pPlayer->PlayerTalkClass->SendCloseGossip();
				return false;
			}
			pPlayer->ADD_GOSSIP_ITEM(4, "I need a pet", GOSSIP_SENDER_MAIN, 1);
			if(pPlayer->CanTameExoticPets())
				pPlayer->ADD_GOSSIP_ITEM(4, "I need an exotic pet", GOSSIP_SENDER_MAIN, 2);
			pPlayer->ADD_GOSSIP_ITEM(4, "Visit the stables", GOSSIP_SENDER_MAIN, 3);
			pPlayer->ADD_GOSSIP_ITEM(4, "I need some items", GOSSIP_SENDER_MAIN, 4);
			pPlayer->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 5);
			pPlayer->PlayerTalkClass->SendGossipMenu(873, pCreature->GetGUID());
			return true;
		}

		bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
		{
			pPlayer->PlayerTalkClass->ClearMenus();

			switch(uiAction)
			{
				case 1:
					pPlayer->ADD_GOSSIP_ITEM(2, "<- Back to Main Menu.", GOSSIP_SENDER_MAIN, 6);
					pPlayer->ADD_GOSSIP_ITEM(4, "Next Page. ->", GOSSIP_SENDER_MAIN, 7);
					pPlayer->ADD_GOSSIP_ITEM(6, "Bat.", GOSSIP_SENDER_MAIN, 8);
					pPlayer->ADD_GOSSIP_ITEM(6, "Bear.", GOSSIP_SENDER_MAIN, 9);
					pPlayer->ADD_GOSSIP_ITEM(6, "Boar.", GOSSIP_SENDER_MAIN, 10);
					pPlayer->ADD_GOSSIP_ITEM(6, "Cat.", GOSSIP_SENDER_MAIN, 11);
					pPlayer->ADD_GOSSIP_ITEM(6, "Carrion Bird.", GOSSIP_SENDER_MAIN, 12);
					pPlayer->ADD_GOSSIP_ITEM(6, "Crab.", GOSSIP_SENDER_MAIN, 13);
					pPlayer->ADD_GOSSIP_ITEM(6, "Crocolisk.", GOSSIP_SENDER_MAIN, 14);
					pPlayer->ADD_GOSSIP_ITEM(6, "Dragonhawk.", GOSSIP_SENDER_MAIN, 15);
					pPlayer->ADD_GOSSIP_ITEM(6, "Gorilla.", GOSSIP_SENDER_MAIN, 16);
					pPlayer->ADD_GOSSIP_ITEM(6, "Hound.", GOSSIP_SENDER_MAIN, 17);
					pPlayer->ADD_GOSSIP_ITEM(6, "Hyena.", GOSSIP_SENDER_MAIN, 18);
					pPlayer->ADD_GOSSIP_ITEM(6, "Moth.", GOSSIP_SENDER_MAIN, 19);
					pPlayer->ADD_GOSSIP_ITEM(6, "Owl.", GOSSIP_SENDER_MAIN, 20);
					pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetGUID());
					break;
				case 2:
					pPlayer->ADD_GOSSIP_ITEM(2, "<- Back to Main Menu.", GOSSIP_SENDER_MAIN, 6);
                    pPlayer->ADD_GOSSIP_ITEM(2, "<- Get a New Normal Pet.", GOSSIP_SENDER_MAIN, 1);
					pPlayer->ADD_GOSSIP_ITEM(6, "Chimaera.", GOSSIP_SENDER_MAIN, 21);
					pPlayer->ADD_GOSSIP_ITEM(6, "Core Hound.", GOSSIP_SENDER_MAIN, 22);
					pPlayer->ADD_GOSSIP_ITEM(6, "Devilsaur.", GOSSIP_SENDER_MAIN, 23);
					pPlayer->ADD_GOSSIP_ITEM(6, "Rhino.", GOSSIP_SENDER_MAIN, 24);
					pPlayer->ADD_GOSSIP_ITEM(6, "Silithid.", GOSSIP_SENDER_MAIN, 25);
					pPlayer->ADD_GOSSIP_ITEM(6, "Worm.", GOSSIP_SENDER_MAIN, 26);  
					pPlayer->ADD_GOSSIP_ITEM(6, "Loque'nahak.", GOSSIP_SENDER_MAIN, 27);
					pPlayer->ADD_GOSSIP_ITEM(6, "Skoll (Wolf).", GOSSIP_SENDER_MAIN, 28);
					pPlayer->ADD_GOSSIP_ITEM(6, "Gondria.", GOSSIP_SENDER_MAIN, 29);
					pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetGUID());
					break;
				case 3:
					pPlayer->GetSession()->SendStablePet(pCreature->GetGUID());
					break;
				case 4:
					pPlayer->GetSession()->SendListInventory(pCreature->GetGUID());
					break;
				case 5:
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				case 6:
					pPlayer->ADD_GOSSIP_ITEM(4, "I need a pet", GOSSIP_SENDER_MAIN, 1);
					if(pPlayer->CanTameExoticPets())
						pPlayer->ADD_GOSSIP_ITEM(4, "I need an exotic pet", GOSSIP_SENDER_MAIN, 2);
					pPlayer->ADD_GOSSIP_ITEM(4, "Visit the stables", GOSSIP_SENDER_MAIN, 3);
					pPlayer->ADD_GOSSIP_ITEM(4, "I need some items", GOSSIP_SENDER_MAIN, 4);
					pPlayer->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 5);
					pPlayer->PlayerTalkClass->SendGossipMenu(654, pCreature->GetGUID());
					break;
				case 7:
					pPlayer->ADD_GOSSIP_ITEM(2, "<- Back to Main Menu.", GOSSIP_SENDER_MAIN, 6);
					pPlayer->ADD_GOSSIP_ITEM(4, "<- Previous Page.", GOSSIP_SENDER_MAIN, 1);
					pPlayer->ADD_GOSSIP_ITEM(6, "Raptor.", GOSSIP_SENDER_MAIN, 30);
					pPlayer->ADD_GOSSIP_ITEM(6, "Ravager.", GOSSIP_SENDER_MAIN, 31);
					pPlayer->ADD_GOSSIP_ITEM(6, "Strider.", GOSSIP_SENDER_MAIN, 32);
					pPlayer->ADD_GOSSIP_ITEM(6, "Scorpid.", GOSSIP_SENDER_MAIN, 33);
					pPlayer->ADD_GOSSIP_ITEM(6, "Spider.", GOSSIP_SENDER_MAIN, 34);
					pPlayer->ADD_GOSSIP_ITEM(6, "Serpent.", GOSSIP_SENDER_MAIN, 35);  
					pPlayer->ADD_GOSSIP_ITEM(6, "Turtle.", GOSSIP_SENDER_MAIN, 36);
					pPlayer->ADD_GOSSIP_ITEM(6, "Wasp.", GOSSIP_SENDER_MAIN, 37);
					pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetGUID());
					break;
				case 8:
					CreatePet(pPlayer, pCreature, 28233);
					break;
				case 9:
					CreatePet(pPlayer, pCreature, 29319);
					break;
				case 10:
					CreatePet(pPlayer, pCreature, 29996);
					break;
				case 11:
					CreatePet(pPlayer, pCreature, 28097);
					break;
				case 12:
					CreatePet(pPlayer, pCreature, 26838);
					break;
				case 13:
					CreatePet(pPlayer, pCreature, 24478);
					break;
				case 14:
					CreatePet(pPlayer, pCreature, 1417);
					break;
				case 15:
					CreatePet(pPlayer, pCreature, 27946);
					break;
				case 16:
					CreatePet(pPlayer, pCreature, 28213);
					break;
				case 17:
					CreatePet(pPlayer, pCreature, 29452);
					break;
				case 18:
					CreatePet(pPlayer, pCreature, 13036);
					break;
				case 19:
					CreatePet(pPlayer, pCreature, 27421);
					break;
				case 20:
					CreatePet(pPlayer, pCreature, 23136);
					break;
				case 21:
					CreatePet(pPlayer, pCreature, 20932);
					break;
				case 22:
					CreatePet(pPlayer, pCreature, 21108);
					break;
				case 23:
					CreatePet(pPlayer, pCreature, 32485);
					break;
				case 24:
					CreatePet(pPlayer, pCreature, 30445);
					break;
				case 25:
					CreatePet(pPlayer, pCreature, 5460);
					break;
				case 26:
					CreatePet(pPlayer, pCreature, 30093);
					break;
				case 27:
					CreatePet(pPlayer, pCreature, 32517);
					break;
				case 28:
					CreatePet(pPlayer, pCreature, 35189);
					break;
				case 29:
					CreatePet(pPlayer, pCreature, 33776);
					break;
				case 30:
					CreatePet(pPlayer, pCreature, 14821);
					break;
				case 31:
					CreatePet(pPlayer, pCreature, 17199);
					break;
				case 32:
					CreatePet(pPlayer, pCreature, 22807);
					break;
				case 33:
					CreatePet(pPlayer, pCreature, 9698);
					break;
				case 34:
					CreatePet(pPlayer, pCreature, 2349);
					break;
				case 35:
					CreatePet(pPlayer, pCreature, 28358);
					break;
				case 36:
					CreatePet(pPlayer, pCreature, 25482);
					break;
				case 37:
					CreatePet(pPlayer, pCreature, 28085);
					break;

			}
			return true;
		}
};



 
void AddSC_Npc_Beastmaster()
{
    new BeastMaster();
}
