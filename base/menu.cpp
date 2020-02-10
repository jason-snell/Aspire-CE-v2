#include "main.h"

float menuX = 250.0f, menuY = 150.0f;
float width = 500.0f, height = 380.0f;
bool moveMenuStarted = false;
bool leftClicked = false;

int textFloat = 0;

MenuBase menu = MenuBase();

char *disconnectStr = "disconnect";

void Menu::createMenu()
{
	menu.addTab("Aimbot");
	menu.addBool("Aimbot", &aimbot->aimbotOn);
	menu.addList("Aimbot Type", &aimbot->aimbotType, AIMBOT_TYPE, 2);
	menu.addList("Target Type", &aimbot->targetType, TARGET_TYPE, 2);
	menu.addBool("No Spread", &aimbot->noSpread);
	menu.addBool("Auto-Shoot", &aimbot->autoShoot);
	menu.addBool("No Recoil", &aimbot->noRecoil);
	menu.addBool("No Sway", &aimbot->noSway);
	menu.addBool("Auto Bone", &aimbot->autoBone);

	menu.addTab("Spinbot");
	menu.addBool("Spinbot", &aimbot->spinbotOn);
	menu.addList("Spinbot X", &aimbot->spinbotX, X_SPINBOT_TYPE, 4);
	menu.addInt("Spinbot X Speed", &aimbot->xSpeed, 1, 70, 1);
	menu.addList("Spinbot Y", &aimbot->spinbotY, Y_SPINBOT_TYPE, 5);
	menu.addInt("Spinbot Y Speed", &aimbot->ySpeed, 1, 180, 1);
	menu.addList("Spinbot Z", &aimbot->spinbotZ, Z_SPINBOT_TYPE, 4);
	menu.addInt("Spinbot Z Speed", &aimbot->zSpeed, 1, 180, 1);

	menu.addTab("ESP");
	menu.addBool("Enemy", &esp->enemy);
	menu.addBool("Friendly", &esp->friendly);
	menu.addList("Snapline Type", &esp->snaplines, SNAPLINE_TYPE, 4);
	menu.addBool("Bones", &esp->bones);
	menu.addBool("Names", &esp->names);
	menu.addList("Box Type", &esp->boxType, BOX_TYPE, 5);
	menu.addBool("Player Weapons", &esp->weaponIcons);
	menu.addBool("Explosive ESP", &esp->explosiveEsp);
	menu.addBool("Dropped Item ESP", &esp->droppedWeaponEsp);
	menu.addSubMenu("ESP Colors", ESPCOLOR_SUBMENU);

	menu.addTab("Misc.");
	menu.addFloat("FOV", &vars->FOV, 65.0f, 120.0f, 1.0f, 1);
	menu.addBool("Edit Safe Area", &vars->editSafeArea);
	menu.addSubMenu("Gamertag Settings", GAMERTAG_SUBMENU);
	menu.addSubMenu("Visuals", VISUALS_SUBMENU);
	menu.addSubMenu("Menu Color", COLOR_SUBMENU);
#if defined (BO2)
	menu.addBool("Spoof IP", &vars->spoofIP);
#elif defined (BO1)
	menu.addButton("Toggle DLC", func::toggleDLC);
#endif
	menu.addButton("End Game", func::endGame);
	menu.addButton("Leave Game", Cbuf_AddText, 0, (int)"disconnect");

	menu.addTab("Player List");
	for (int i = 0; i < 18; i++)
	{
		menu.addSubMenu("Client", PLAYER_SUBMENU + i);
	}

	menu.createSubMenu("ESP Colors", ESPCOLOR_SUBMENU);
	menu.addSubMenu("Friendly Color", FRIENDLY_COLOR);
	menu.addSubMenu("Enemy Color", ENEMY_COLOR);
	menu.addSubMenu("Enemy Hittable", ENEMY_HITTABLE);
	menu.addSubMenu("Enemy Visible", ENEMY_VISIBLE);
	menu.addSubMenu("Whitelisted Color", WHITELISTED_COLOR);
	menu.addSubMenu("Prioritized Color", PRIORITIZED_COLOR);

	menu.createSubMenu("Friendly Color", FRIENDLY_COLOR);
	menu.addFloat("Friendly Color Red", &friendColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Friendly Color Green", &friendColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Friendly Color Blue", &friendColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Friendly Color Opacity", &friendColor[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Enemy Color", ENEMY_COLOR);
	menu.addFloat("Enemy Color Red", &enemColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Color Green", &enemColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Color Blue", &enemColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Color Opacity", &enemColor[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Enemy Hittable", ENEMY_HITTABLE);
	menu.addFloat("Enemy Hittable Color Red", &hittableColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Hittable Color Green", &hittableColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Hittable Color Blue", &hittableColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Hittable Color Opacity", &hittableColor[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Enemy Visible", ENEMY_VISIBLE);
	menu.addFloat("Enemy Visible Color Red", &visibleColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Visible Color Green", &visibleColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Visible Color Blue", &visibleColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Enemy Visible Color Opacity", &visibleColor[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Whitelisted Color", WHITELISTED_COLOR);
	menu.addFloat("Whitelisted Color Red", &ignoredColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Whitelisted Color Green", &ignoredColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Whitelisted Color Blue", &ignoredColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Whitelisted Color Opacity", &ignoredColor[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Prioritized Color", PRIORITIZED_COLOR);
	menu.addFloat("Prioritized Color Red", &targetedClient[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Prioritized Color Green", &targetedClient[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Prioritized Color Blue", &targetedClient[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Prioritized Color Opacity", &targetedClient[3], 0.0f, 1.0f, 0.01f, 2);

	menu.createSubMenu("Gamertag Settings", GAMERTAG_SUBMENU);
	menu.addButton("Set Custom Gamertag", func::setupEnterCustomTag);
	menu.addBool("Rainbow Gamertag", &vars->rainbowGamertag);
	menu.addInt("Rainbow Delay", &vars->rainbowSpeed, 1, 15, 1);
	menu.addBool("Cycle Gamertags", &vars->cycleGamertags);
	menu.addOption("Gamertag Preset #1", PresetGamertag[0]);
	menu.addOption("Gamertag Preset #2", PresetGamertag[1]);
	menu.addOption("Gamertag Preset #3", PresetGamertag[2]);
	menu.addOption("Gamertag Preset #4", PresetGamertag[3]);
	menu.addOption("Gamertag Preset #5", PresetGamertag[4]);
	menu.addInt("Cycle Delay", &vars->gamertagCycleDelay, 1, 10, 1);

	menu.createSubMenu("Visuals", VISUALS_SUBMENU);
	menu.addBool("FPS Counter", &vars->fps);
	menu.addList("Host Info", &vars->hostName, HOSTNAME_TYPE, 4);
	menu.addBool("Host Geo Location", &vars->hostGeoLocation);
	menu.addBool("Map Name", &vars->mapName);
	menu.addBool("Health Bar", &vars->healthBar);
	menu.addBool("Crosshair", &vars->crosshair);
	menu.addBool("Controls", &vars->controls);

	menu.createSubMenu("Menu Color", COLOR_SUBMENU);
	menu.addSubMenu("Primary Color", MAIN_COLOR);
	menu.addSubMenu("Secondary Color", FONT_COLOR);
	menu.addSubMenu("Background Color", BACKGROUND_COLOR);
	menu.addBool("Font Shadow", &vars->fontShadow);

	menu.createSubMenu("Primary Color", MAIN_COLOR);
	menu.addFloat("Primary Color Red", &darkMenuColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Primary Color Green", &darkMenuColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Primary Color Blue", &darkMenuColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Primary Color Opacity", &darkMenuColor[3], 0.0f, 1.0f, 0.01f, 2);
	menu.addBool("Primary Color Fade", &vars->primaryColorRainbow);
	menu.addFloat("Primary Color Fade Speed", &vars->pimaryRainbowSpeed, 0.001f, 0.1f, 0.001f, 3);

	menu.createSubMenu("Secondary Color", FONT_COLOR);
	menu.addFloat("Secondary Color Red", &secondaryColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Secondary Color Green", &secondaryColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Secondary Color Blue", &secondaryColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Secondary Color Opacity", &secondaryColor[3], 0.0f, 1.0f, 0.01f, 2);
	menu.addBool("Secondary Color Fade", &vars->secondaryColorRainbow);
	menu.addFloat("Secondary Color Fade Speed", &vars->secondaryRainbowSpeed, 0.0001f, 0.5f, 0.01f, 3);


	menu.createSubMenu("Background Color", BACKGROUND_COLOR);
	menu.addFloat("Background Color Red", &lightMenuColor[0], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Background Color Green", &lightMenuColor[1], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Background Color Blue", &lightMenuColor[2], 0.0f, 1.0f, 0.01f, 2);
	menu.addFloat("Background Color Opacity", &lightMenuColor[3], 0.0f, 1.0f, 0.01f, 2);
	menu.addBool("Background Color Fade", &vars->backgroundColorRainbow);
	menu.addFloat("Background Color Fade Speed", &vars->backgroundRainbowSpeed, 0.0001f, 0.5f, 0.01f, 3);

	for (int i = 0; i < 18; i++)
	{
		menu.createSubMenu("Client", PLAYER_SUBMENU + i);
		menu.addOption("Gamertag", "Empty...");
		menu.addBool("Prioritize", &aimbot->prioritized[i]);
		menu.addBool("Whitelist", &aimbot->whitelisted[i]);
		menu.addButton("Face Sheild At Client", func::setClientShield, i);
		menu.addOption("XUID", "Empty...");
		menu.addOption("IP", "Empty...");
		menu.addOption("Country", "Empty...");
		menu.addOption("State", "Empty...");
		menu.addOption("City", "Empty...");
		menu.addOption("Zip Code", "Empty...");
		menu.addOption("ISP", "Empty...");
		menu.addOption("Mobile", "False");
		menu.addOption("Proxy", "False");
	}
}


void Menu::drawMenu()
{
	menuX = (vars->width / 2.0f) - (width / 2.0f);
	menuY = (vars->height / 2.0f) - (height / 2.0f) - 30.0f;

	height = 80.0f + (menu.menuOptionIndex[menu.currentMenu] - 1) * 25.0f;
	//height - 25.0f
	draw::rect(menuX, menuY + 5.0f, width, 20.0f, "white", darkMenuColor, false); // Move Bar
	draw::text("Aspire CE", menuX + (width / 2.0f), menuY + 4.0f, menu.returnFont(), menu.returnFontSize() + 0.1f, secondaryColor, 'C', 0.0f, vars->fontShadow ? 6 : 0); // Title
	draw::rect(menuX, menuY + 25.0f, width, 5.0f, "white", lightMenuColor, false); // Background
	draw::rect(menuX, menuY + 50.0f, width, 1.0f, "white", darkMenuColor);
	draw::rect(menuX, menuY + 30.0f, width, 15.0f, "white", darkMenuColor);
	draw::rect(menuX, menuY + 45.0f, width, 5.0f, "white", lightMenuColor, false); // Background
	draw::rect(menuX, menuY + 51.0f, width, (54.0f + (menu.Scroll[menu.currentMenu] * 25.0f)) - 51.0f, "white", lightMenuColor, false); // Background
	draw::rect(menuX, menuY + 54.0f + (menu.Scroll[menu.currentMenu] * 25.0f), width, 20.0f, "white", darkMenuColor, false); // Scroll Bar
	draw::rect(menuX, menuY + 54.0f + (menu.Scroll[menu.currentMenu] * 25.0f) + 20.0f, width, (height - 25.0f) - 54.0f - (menu.Scroll[menu.currentMenu] * 25.0f) + 10.0f, "white", lightMenuColor, false); // Background

	float tabWidth = width / 5.0f;

	for (int i = 0; i < 5; i++)
	{
#if defined(BO2)
		draw::text(menu.tabText[i], menuX + (tabWidth * i) + (tabWidth / 2.0f), menuY + 28.0f, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'C', 0.0f, vars->fontShadow ? 6 : 0);
#else
		draw::text(menu.tabText[i], menuX + (tabWidth * i) + (tabWidth / 2.0f), menuY + 28.0f, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'C', 0.0f, vars->fontShadow ? 6 : 0);
#endif
	}

	menu.menuOptionIndex[4] = Dvar_GetInt("party_maxplayers");
	if (menu.currentMenu == 4)
	{
		if(menu.Scroll[menu.currentMenu] > Dvar_GetInt("party_maxplayers"))
			menu.Scroll[menu.currentMenu] = 0;
	}
		

	char listBuffer[64] = { 0 };
	for (int i = menu.menuOptionIndex[menu.currentMenu] - 1; i >= 0; i--)
	{
		if (menu.currentMenu == 4)
		{
			char *Gamertag = func::inGame() ? cg->clientInfo_t[i].clientName : (char*)(addr->netInfo + (i * addr->netInfoSize) + addr->netInfoGamertag);
			if (Gamertag && *(int*)Gamertag != 0)
				menu.menuText[menu.currentMenu][i] = Gamertag;
			else
				menu.menuText[menu.currentMenu][i] = "Empty...";
		}
		else if (menu.currentMenu >= PLAYER_SUBMENU)
		{
			char *Gamertag = func::inGame() ? cg->clientInfo_t[(menu.currentMenu - PLAYER_SUBMENU)].clientName : (char*)(addr->netInfo + ((menu.currentMenu - PLAYER_SUBMENU) * addr->netInfoSize) + addr->netInfoGamertag);
			unsigned char *IP = (unsigned char*)(addr->netInfo + ((menu.currentMenu - PLAYER_SUBMENU) * addr->netInfoSize) + addr->netInfoIP);
			unsigned long long XUID = *(unsigned long long*)(addr->netInfo + ((menu.currentMenu - PLAYER_SUBMENU) * addr->netInfoSize) + addr->netInfoXUID);
			if (Gamertag && *(int*)Gamertag != 0)
			{
				menu.optionSecondText[menu.currentMenu][0] = Gamertag;

				if (IP[0] == 0)
				{
					menu.optionSecondText[menu.currentMenu][4] = "Empty...";
					menu.optionSecondText[menu.currentMenu][5] = "Empty...";
					menu.optionSecondText[menu.currentMenu][6] = "Empty...";
					menu.optionSecondText[menu.currentMenu][7] = "Empty...";
					menu.optionSecondText[menu.currentMenu][8] = "Empty...";
					menu.optionSecondText[menu.currentMenu][9] = "Empty...";
					menu.optionSecondText[menu.currentMenu][10] = "Empty...";
					menu.optionSecondText[menu.currentMenu][11] = "Empty...";
					menu.optionSecondText[menu.currentMenu][12] = "Empty...";
				}
				else
				{
					char sIP[32] = { 0 };
					sprintf_s(sIP, "%i.%i.%i.%i", IP[0], IP[1], IP[2], IP[3]);
					char sXUID[32] = { 0 };
					sprintf_s(sXUID, "000%llX", XUID);

					menu.optionSecondText[menu.currentMenu][4] = sXUID;
					menu.optionSecondText[menu.currentMenu][5] = sIP;
					menu.optionSecondText[menu.currentMenu][6] = (char*)Country[menu.currentMenu - PLAYER_SUBMENU];
					menu.optionSecondText[menu.currentMenu][7] = (char*)State[menu.currentMenu - PLAYER_SUBMENU];
					menu.optionSecondText[menu.currentMenu][8] = (char*)City[menu.currentMenu - PLAYER_SUBMENU];
					menu.optionSecondText[menu.currentMenu][9] = (char*)ZipCode[menu.currentMenu - PLAYER_SUBMENU];
					menu.optionSecondText[menu.currentMenu][10] = (char*)ISP[menu.currentMenu - PLAYER_SUBMENU];
					menu.optionSecondText[menu.currentMenu][11] = MOBILE[menu.currentMenu - PLAYER_SUBMENU] ? "True" : "False";
					menu.optionSecondText[menu.currentMenu][12] = PROXY[menu.currentMenu - PLAYER_SUBMENU] ? "True" : "False";
				}
			}
			else
			{
				menu.optionSecondText[menu.currentMenu][4] = "Empty...";
				menu.optionSecondText[menu.currentMenu][5] = "Empty...";
				menu.optionSecondText[menu.currentMenu][6] = "Empty...";
				menu.optionSecondText[menu.currentMenu][7] = "Empty...";
				menu.optionSecondText[menu.currentMenu][8] = "Empty...";
				menu.optionSecondText[menu.currentMenu][9] = "Empty...";
				menu.optionSecondText[menu.currentMenu][10] = "Empty...";
				menu.optionSecondText[menu.currentMenu][11] = "Empty...";
				menu.optionSecondText[menu.currentMenu][12] = "Empty...";
			}
		}

		static bool sliderClicked[30] = { false }, dropDownClicked[30] = { false };
		draw::text(menu.menuText[menu.currentMenu][i], menuX + 10.0f, menuY + 55.0f + (i * 25.0f), menu.returnFont(), menu.returnFontSize(), secondaryColor, 'L', 0.0f, vars->fontShadow ? 6 : 0);
		switch (menu.optionType[menu.currentMenu][i])
		{
		case TYPE_OPTION:
			if(menu.optionSecondText[menu.currentMenu][i] != 0)
				draw::text(menu.optionSecondText[menu.currentMenu][i], menuX + (width - 10.0f), menuY + 55.0f + (i * 25.0f), menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, vars->fontShadow ? 6 : 0);
			break;
		case TYPE_OPTION_BOOL:
			draw::checkBox(menuX + (width - 28.0f), menuY + 56.0f + (i * 25.0f), *menu.menuBools[menu.currentMenu][i], darkMenuColor);
			break;
		case TYPE_OPTION_LIST:
			sprintf_s(listBuffer, "%s (%i/%i)", menu.lists[menu.currentMenu][i][*menu.listInts[menu.currentMenu][i]], *menu.listInts[menu.currentMenu][i] + 1, menu.listOptionCount[menu.currentMenu][i] + 1);
			draw::text(listBuffer, menuX + (width - 10.0f), menuY + 55.0f + (i * 25.0f), menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, vars->fontShadow ? 6 : 0);
			break;
		case TYPE_OPTION_INT:
			draw::intSlider(menuX + (width - 110.0f), menuY + 59.0f + (i * 25.0f), *menu.listInts[menu.currentMenu][i], menu.intMin[menu.currentMenu][i], menu.intMax[menu.currentMenu][i]);
			break;
		case TYPE_OPTION_FLOAT:
			draw::floatSlider(menuX + (width - 110.0f), menuY + 59.0f + (i * 25.0f), *menu.listFloats[menu.currentMenu][i], menu.fMin[menu.currentMenu][i], menu.fMax[menu.currentMenu][i], menu.fDecPlace[menu.currentMenu][i]);
			break;
		case TYPE_OPTION_CALL:
			break;
		case TYPE_OPTION_SUBMENU:
			draw::text(">", menuX + (width - 10.0f), menuY + 55.0f + (i * 25.0f), menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, vars->fontShadow ? 6 : 0);
			break;
		}
	}
	int indx = 0;
	if (menu.currentMenu > PLAYER_SUBMENU - 1)
		indx = 4;
	else if (menu.currentMenu == GAMERTAG_SUBMENU || menu.currentMenu == COLOR_SUBMENU || menu.currentMenu == MAIN_COLOR || menu.currentMenu == FONT_COLOR || menu.currentMenu == BACKGROUND_COLOR || menu.currentMenu == VISUALS_SUBMENU)
		indx = 3;
	else if (menu.currentMenu == ESPCOLOR_SUBMENU || menu.currentMenu == FRIENDLY_COLOR || menu.currentMenu == ENEMY_COLOR || menu.currentMenu == ENEMY_HITTABLE || menu.currentMenu == ENEMY_VISIBLE)
		indx = 2;
	else indx = menu.currentMenu;
	draw::rect(menuX + (tabWidth * indx) + 5.0f, menuY + 44.0f, tabWidth - 10.0f, 1.0f, "white", secondaryColor);
}