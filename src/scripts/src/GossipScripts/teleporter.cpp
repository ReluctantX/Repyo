class Item_Teleporter : public Arcemu::Gossip::Script
{
   public:
	   void OnHello(Object * pObject, Player * Plr)
	   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   if(Plr->IsDead())
			{	
				Plr->ResurrectPlayer();
				Plr->Gossip_Complete();
			}
		   else
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "City Teleport", 1);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Worldmap", 5);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Instances & Raids", 3);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Custom Instances", 110);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Mission Area", 180);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "The Mall", 180);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "PVP Zones", 3);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Event Areas", 3);
		   menu.AddItem(Arcemu::Gossip::ICON_DOT, "Player Services", 1200);
		   menu.Send(Plr);
	   }

	   void OnSelectOption(Object * pObject, Player * Plr, uint32 Id, const char * code)
	   {
		   GossipMenu * Menu;
		   switch(Id)
		   {
			   // Main Cities & Dalaran -- Start - works diffrentish to ascent :P
		      case 1:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
					 Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Stormwind", 101);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Ironforge", 102);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Darnassus", 103);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Exodar", 104);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Orgrimmar", 105);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Thunder Bluff", 106);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Undercity", 107);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Silvermoon City", 108);
					  menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Dalaran", 109);
					  menu.Send(Plr);
			  }

			  break;

			  // Player Services - Start

			  case 1200:
				  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
					 Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
					  menu.AddItem(Arcemu::Gossip::ICON_DOT, "View my Personal Bank", 1201);
					  menu.AddItem(Arcemu::Gossip::ICON_DOT, "Set my Hearthstone location here", 1202);
					  menu.AddItem(Arcemu::Gossip::ICON_DOT, "Advance all my skills to 450", 1203);
					  menu.Send(Plr);
			  }

			  break;

			  case 1201:
				  Plr->GetSession()->SendBankerList(TO_CREATURE(pObject));
				  break;

			  case 1202:
				  Plr->GetSession()->SendInnkeeperBind(TO_CREATURE(pObject));
				  Plr->BroadcastMessage("You may need to use this feature again for your new hearthstone location to be changed...");
				  break;

			  case 1203:
		     if (Plr->_HasSkillLine(43))
             {Plr->_AdvanceSkillLine(43, 450);}
              if (Plr->_HasSkillLine(55))
              {Plr->_AdvanceSkillLine(55, 450);}
              if (Plr->_HasSkillLine(44))
              {Plr->_AdvanceSkillLine(44, 450);}
              if (Plr->_HasSkillLine(95))
              {Plr->_AdvanceSkillLine(95, 450);}
              if (Plr->_HasSkillLine(54))
              {Plr->_AdvanceSkillLine(54, 450);}
              if (Plr->_HasSkillLine(45))
              {Plr->_AdvanceSkillLine(45, 450);}
              if (Plr->_HasSkillLine(46))
              {Plr->_AdvanceSkillLine(46, 450);}
              if (Plr->_HasSkillLine(136))
              {Plr->_AdvanceSkillLine(136, 450);}
              if (Plr->_HasSkillLine(160))
              {Plr->_AdvanceSkillLine(160, 450);}
              if (Plr->_HasSkillLine(162))
              {Plr->_AdvanceSkillLine(162, 450);}
              if (Plr->_HasSkillLine(172))
              {Plr->_AdvanceSkillLine(172, 450);}
              if (Plr->_HasSkillLine(173))
              {Plr->_AdvanceSkillLine(173, 450);}
              if (Plr->_HasSkillLine(176))
              {Plr->_AdvanceSkillLine(176, 450);}
              if (Plr->_HasSkillLine(226))
              {Plr->_AdvanceSkillLine(226, 450);}
              if (Plr->_HasSkillLine(228))
              {Plr->_AdvanceSkillLine(228, 450);}
              if (Plr->_HasSkillLine(229))
              {Plr->_AdvanceSkillLine(229, 450);}
              if (Plr->_HasSkillLine(473))
              {Plr->_AdvanceSkillLine(473, 450);}
			  {Plr->_AdvanceAllSkills(450);}
			  Plr->BroadcastMessage("All skills are now advanced to 450");
              Plr->Gossip_Complete();
                         break;

			// Player Services -- END

			
			  // World Locations -- Start

			  case 5:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Eastern Kingdom Locations", 7);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Kilimandor Locations", 9);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Outland Locations", 11);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Northrend Locations", 12);
		   menu.Send(Plr);
			  }

			  break;

			  case 7:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Eversong Woods", 13);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Ghostlands", 14);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Dun Morogh", 15);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Westfall", 16);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Stranglethorn Vale", 17);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Wetlands", 18);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Tirisfal Glades", 19);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Isle of Quel'Danas", 20);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Eastern Plaguelands", 21);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "The Hinterlands", 22);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, " Page 2 ------>", 8);
		   menu.Send(Plr);
			  }

			  break;

			  case 8:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Arathi Highlands", 23);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Hillsbrad Foothills", 24);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Elwynn Forest", 25);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Blasted Lands", 26);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Western Plaguelands", 27);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Slverpine Forest", 28);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Duskwood", 29);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Swamp of Sorrows", 30);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Burning Steppes", 31);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Alterac Mountains", 32);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Redridge Mountains", 33);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, " <------ Page 1", 7);
		   menu.Send(Plr);
			  }

			  break;

			  case 9:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Teldrassil", 34);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Bloodmyst Isle", 35);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Azuremyst Isle", 36);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Tanaris", 37);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Feralas", 38);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Azshara", 39);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Durotar", 40);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Stonetalon Mountains", 41);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Dustwallow Marsh", 42);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Silithus", 43);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Un'Goro Crater", 44);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Page 2 ------>", 10);
		   menu.Send(Plr);
			  }

			  break;

			  case 10:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "The Barrens", 45);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Darkshore", 46);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Ashenvale", 47);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Winterspring", 48);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Thousand Needles", 49);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Desolace", 50);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Felwood", 51);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Moonglade", 52);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, " <------ Page 1", 9);
		   menu.Send(Plr);
			  }

			  break;

			  case 11:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Hellfire Peninsula", 53);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Zangamarsh", 54);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Trokkar Forest", 55);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Nagrand", 56);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Shadowmoon Valley", 57);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Blade's Edge Mountains", 58);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Netherstorm", 59);
		   menu.Send(Plr);
			  }

			  break;

			  case 12:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Borean Tundra", 60);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Zul'Drak", 61);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Icecrown", 62);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Grizzley Hills", 63);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Dragonblight", 64);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Howling Fjord", 65);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Sholazar Basin", 66);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Crystalsong Forest", 67);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Wintergrasp", 68);
		   menu.Send(Plr);
			  }

			  break;

			  case 69:
				  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Database Reloader", 70);
		   menu.Send(Plr);
			  }

			  break;

			  case 70:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT_11, "Reload Items Table", 71);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000Return to Main Menu|r", 999);
		   menu.Send(Plr);
			  }
		
			  case 71: // Items
                                            char str[200];
                                                snprintf(str, 200, "%s%s initiated server-side reload of table 'items',The server may experience some lag while this occurs.",
                MSG_COLOR_LIGHTRED, Plr->GetName());									
                                                sWorld.SendWorldText(str, 0);
                                                ItemPrototypeStorage.Reload();
                                                Plr->BroadcastMessage("|cff9ACD32 Please wait 10 seconds for your table to reload.|r");
												Arcemu::Gossip::Menu::Complete(Plr);
                                        break;

			  // World Locations Ports Start

			  case 13:
				   Plr->EventTeleport(530, 8041.913086f, -6883.237793f, 58.292896f);
				   Plr->SetFacing(6.177588f); // Eversong Woods
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 33333:
				  {
				     int lenght;
					 lenght = strlen(code);
					 if(lenght = 123)
					 {
						 objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
						 {
							 Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
							 menu.AddItem(10, "Hello Again", 2222);
							 menu.Send(Plr);
						 }
					 }
					 else
					 {
						 Plr->BroadcastMessage("Invalid code");
					 }
				  
				  }break;

			  case 14:
				   Plr->EventTeleport(530, 6127.694336f, -7007.258789f, 139.115814f);
				   Plr->SetFacing(5.848654f); // Ghostlands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 15:
				   Plr->EventTeleport(0, -5632.823730f, -2234.718018f, 424.754456f);
				   Plr->SetFacing(1.214007f); // Dun Morgh
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 16:
				   Plr->EventTeleport(0, -9835.950195f, 879.68646f, 26.678028f);
				   Plr->SetFacing(2.064594f); // Westfall
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 17:
				   Plr->EventTeleport(0, -11393.176758f, -286.610626f, 58.885357f);
				   Plr->SetFacing(3.140592f); // Stranglethorn Vale
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 18:
				   Plr->EventTeleport(0, -4086.305176f, -2626.833740f, 43.062832f);
				   Plr->SetFacing(4.696673f); // Wetlands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 19:
				   Plr->EventTeleport(0, 1705.972534f, -695.993408f, 51.121151f);
				   Plr->SetFacing(1.694098f); // Tirisfal Glades
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 20:
				   Plr->EventTeleport(530, 12926.485352f, -6907.943848f, 4.813465f);
				   Plr->SetFacing(3.773059f); // Isle of Quel'Danas
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 21:
				   Plr->EventTeleport(0, 1923.329834f, -2665.596924f, 60.108654f);
				   Plr->SetFacing(4.729665f); // Eastern Plaguelands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 22:
				   Plr->EventTeleport(0, 68.039368f, -1953.572021f, 155.825958f);
				   Plr->SetFacing(0.206539f); // The Hinterlands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 23:
				   Plr->EventTeleport(0, -898.894531f, -1646.272949f, 48.220856f);
				   Plr->SetFacing(3.654434f); // Arathi Highlands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 24:
				   Plr->EventTeleport(0, -776.134399f, -1498.186890f, 58.630692f);
				   Plr->SetFacing(1.188284f); // Hillsbrad Foothills
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 25:
				   Plr->EventTeleport(0, -9773.393555f, 799.036377f, 24.947439f);
				   Plr->SetFacing(5.309263f); // Elwynn Forest
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 26:
				   Plr->EventTeleport(0, -10784.281250f, -2995.632324f, 47.059845f);
				   Plr->SetFacing(2.981631f); // Blasted Lands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 27:
				   Plr->EventTeleport(0, 1718.960205f, -844.387268f, 60.477154f);
				   Plr->SetFacing(4.609754f); // Western Plaguelands
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 28:
				   Plr->EventTeleport(0, 1404.229126f, 769.978088f, 47.242317f);
				   Plr->SetFacing(1.935057f); // Slverpine Forest
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 29:
				   Plr->EventTeleport(0, -10859.000977f, 552.059631f, 30.488665f);
				   Plr->SetFacing(4.908446f); // Duskwood
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 30:
				   Plr->EventTeleport(0, -10395.317383f, -2470.308594f, 39.031433f);
				   Plr->SetFacing(4.386249f); // Swamp of Sorrows
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 31:
				   Plr->EventTeleport(0, -8326.839844f, -2533.875732f, 133.147476f);
				   Plr->SetFacing(6.162805f); // Burning Steppes
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 32:
				   Plr->EventTeleport(0, 417.606598f, -616.347900f, 164.692810f);
				   Plr->SetFacing(6.057762f); // Alterac Mountains
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 33:
				   Plr->EventTeleport(0, -9605.413086f, -1859.599609f, 56.1392f);
				   Plr->SetFacing(4.386249f); // Redridge Mountains
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 34:
				   Plr->EventTeleport(1, 10109.257813f, 1759.851563f, 1335.233398f);
				   Plr->SetFacing(4.531741f); // Teldrassil
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 35:
				   Plr->EventTeleport(530, -2736.895996f, -12207.879883f, 8.780541f);
				   Plr->SetFacing(0.197909f); // Bloodmyst Isle
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 36:
				   Plr->EventTeleport(530, -4134.761719f, -12192.087891f, -1.028379f);
				   Plr->SetFacing(4.238788f); // Azuremyst Isle
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 37:
				   Plr->EventTeleport(1, -7051.750000f, -3773.560059f, 10.311100f);
				   Plr->SetFacing(1.225841f); // Tanaris
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 38:
				   Plr->EventTeleport(1, -4305.569336f, -72.761520f, 62.382816f);
				   Plr->SetFacing(1.912435f); // Ferala
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 39:
				   Plr->EventTeleport(1, 2741.443604f, -3961.385010f, 89.392708f);
				   Plr->SetFacing(4.806637f); // Azshara
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 40:
				   Plr->EventTeleport(1, 306.597992f, -3841.580078f, 25.829679f);
				   Plr->SetFacing(4.490906f); // Durotar
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 41:
				   Plr->EventTeleport(1, 153.481201f, 36.512650f, 29.961561f);
				   Plr->SetFacing(1.429383f); // Stonetalon Mountains
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 42:
				   Plr->EventTeleport(1, -3691.857666f, -2611.938232f, 49.742939f);
				   Plr->SetFacing(5.389327f); // Dustwallow Marsh
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 43:
				   Plr->EventTeleport(1, -6715.415039f, 14.269995f, 1.241704f);
				   Plr->SetFacing(2.192252f); // Silithus
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 44:
				   Plr->EventTeleport(1, -7879.322754f, -2118.781250f, -268.507202f);
				   Plr->SetFacing(0.951308f); // Un'Goro Crater
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 45:
				   Plr->EventTeleport(1, 715.104431f, -2322.595947f, 91.667679f);
				   Plr->SetFacing(4.197953f); // The Barrens
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 46:
				   Plr->EventTeleport(1, 4488.048340f, 269.988373f, 59.374416f);
				   Plr->SetFacing(0.297667f); // Darkshore
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 47:
				   Plr->EventTeleport(1, 3974.773438f, -2.935500f, 16.973385f);
				   Plr->SetFacing(2.693920f); // Ashenvale
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 48:
				   Plr->EventTeleport(1, 6855.680176f, -2311.294922f, 580.135986f);
				   Plr->SetFacing(3.145515f); // Winterspring
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 49:
				   Plr->EventTeleport(1, -4832.747070f, -1813.980469f, -53.396034f);
				   Plr->SetFacing(0.699003f); // Thousand Needles
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 50:
				   Plr->EventTeleport(1, 264.103302f, 1836.261597f, 86.229088f);
				   Plr->SetFacing(3.467549f); // Desolace
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 51:
				   Plr->EventTeleport(1, 3526.419189f, -1516.115846f, 166.402771f);
				   Plr->SetFacing(0.035336f); // Felwood
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 52:
				   Plr->EventTeleport(1, 7999.680176f, -2670.199951f, 512.099792f);
				   Plr->SetFacing(0.938545f); // Moonglade
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 53:
				   Plr->EventTeleport(530, -245.797897f, 5082.710449f, 74.657898f);
				   Plr->SetFacing(4.700608f); // Hellfire Peninsula
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 54:
				   Plr->EventTeleport(530, -234.343262f, 5336.653320f, 24.003939f);
				   Plr->SetFacing(1.201655f); // Zangamarsh
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 55:
				   Plr->EventTeleport(530, -1866.603638f, 4255.366699f, 10.3500298f);
				   Plr->SetFacing(2.135281f); // Trokkar Forest
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 56:
				   Plr->EventTeleport(530, -593.996339f, 8444.159180f, 63.993176f);
				   Plr->SetFacing(3.302601f); // Nagrand
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 57:
				   Plr->EventTeleport(530, -3072.878906f, 2885.328857f, 82.354279f);
				   Plr->SetFacing(4.158682f); // Shadowmoon Valley
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 58:
				   Plr->EventTeleport(530, 1501.536377f, 6820.001465f, 110.724258f);
				   Plr->SetFacing(5.969026f); // Blade's Edge Mountains
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 59:
				   Plr->EventTeleport(530, 3388.016357f, 4214.859863f, 125.598015f);
				   Plr->SetFacing(1.797915f); // Netherstorm
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 60:
				   Plr->EventTeleport(571, 3521.859863f, 3262.111328f, 24.957090f);
				   Plr->SetFacing(5.494126f); // Borean Tundra
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 61:
				   Plr->EventTeleport(571, 4839.282227f, -1522.459839f, 248.962860f);
				   Plr->SetFacing(5.494126f); // Zul'Drak
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 62:
				   Plr->EventTeleport(571, 6572.830078f, 405.394745f, 416.026733f);
				   Plr->SetFacing(1.036726f); // Icecrown
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 63:
				   Plr->EventTeleport(571, 4542.711914f, -3450.444336f, 226.930756f);
				   Plr->SetFacing(3.777763f); // Grizzley Hills
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 64:
				   Plr->EventTeleport(571, 3506.822510f, 2883.356201f, 30.721956f);
				   Plr->SetFacing(4.755583f); // Dragonblight
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 65:
				   Plr->EventTeleport(571, 2274.427002f, -3374.704590f, 148.085632f);
				   Plr->SetFacing(3.467536f); // Howling Fjord
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 66:
				   Plr->EventTeleport(571, 4918.335938f, 5495.261719f, -77.366486f);
				   Plr->SetFacing(1.511895f); // Sholazar Basin
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 67:
				   Plr->EventTeleport(571, 5353.132813f, 124.632469f, 161.554062f);
				   Plr->SetFacing(5.583393f); // Crystalsong Forest
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 68:
				   Plr->EventTeleport(571, 5354.265137f, 2839.642578f, 409.239563f);
				   Plr->SetFacing(3.176145f); // Wintergrasp
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  // World Locations ports End

			  case 998:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "COMING SOON", 101);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000Return to Main Menu|r", 999);
		   menu.Send(Plr);
			  }

			  break;

			  case 180:
				   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			   {
		   Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000[NEW]|r |cff0000ffDustwalker|r\n", 181);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000[COMING SOON]|r ------\n", 270);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000[COMING SOON]|r ------\n", 270);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000[COMING SOON]|r ------\n", 270);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000[COMING SOON]|r ------\n", 207);
		   menu.AddItem(Arcemu::Gossip::ICON_CHAT, "|cffff0000Return to Main Menu|r", 999);
		   menu.Send(Plr);
			  }

			   break;

			   case 181:
				   Plr->EventTeleport(36, -1661.536255f, 472.988495f, 2.577062f); // Dustwalker
				   Plr->SetFacing(0.009865f);
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;
			   //Main City locations - Horde, Alliance & Dalaran

			  case 101:
				   Plr->EventTeleport(0, -8904.405273f, 560.943054f, 93.163231f); // Stormwind
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 102:
				   Plr->EventTeleport(0, -5026.468750f, -827.388245f,495.295776f); // Ironforge
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 103:
				   Plr->EventTeleport(1, 9945.720703f, 2604.302734f, 1316.188599f); // Darnassus
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 104:
				   Plr->EventTeleport(530, -3982.408203f, -11834.411133f, -7.042034f); // Exodar
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 105:
				   Plr->EventTeleport(1, 1375.108643f, -4369.260254f, 26.029215f); // Orgrimmar
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 106:
				   Plr->EventTeleport(1, -1284.158154f, 147.915771f, 129.671402f); // Thunder bluff
				  Arcemu::Gossip::Menu::Complete(Plr); 
			  break;

			  case 107:
				   Plr->EventTeleport(0, 1843.476075f, 238.787292f, 62.675154f); // Undercity
				  Arcemu::Gossip::Menu::Complete(Plr);; 
			  break;

			  case 108:
				   Plr->EventTeleport(530, 9461.446289f, -7278.271973f, 14.195700f); // Silvermoon City
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 109:
				   Plr->EventTeleport(571, 5793.126465f, 625.577576f, 647.392212f); // Dalaran
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  // Defuse Locations (Mall, Mission Zone ect...)

			  case 110:
				   Plr->EventTeleport(530, -248.935883f, 966.172668f, 84.334129f); // Mission Zone
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;

			  case 111:
				   Plr->EventTeleport(571, 5793.126465f, 625.577576f, 647.392212f); // Dalaran
				  Arcemu::Gossip::Menu::Complete(Plr);
				  Plr->BroadcastMessage("You're now being teleported"); 
			  break;

			  case 112:
				   Plr->EventTeleport(571, 5793.126465f, 625.577576f, 647.392212f); // Dalaran
				  Arcemu::Gossip::Menu::Complete(Plr);
				  Plr->BroadcastMessage("You're now being teleported"); 
			  break;

			  case 113:
				   Plr->EventTeleport(571, 5793.126465f, 625.577576f, 647.392212f); // Dalaran
				  Arcemu::Gossip::Menu::Complete(Plr);
				  Plr->BroadcastMessage("You're now being teleported"); 
			  break;

			  // World locations (EK, Kil, OL and Northrend)

			  case 9993:
				   Plr->EventTeleport(571, 5793.126465f, 625.577576f, 647.392212f); // Dalaran
				  Arcemu::Gossip::Menu::Complete(Plr);
			  break;
		   }
	   }
};

void SetupTeleporter(ScriptMgr * mgr)
{
	mgr->register_item_gossip(7871, new Item_Teleporter);
}