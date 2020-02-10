#include "main.h"

MenuBase::MenuBase()
{

}

MenuBase::~MenuBase()
{

}

void MenuBase::addTab(char *text)
{
	tabText[tabIndex] = text;
	tabIndex++;
}

void MenuBase::addSubMenu(char *subMenuName, int subMenu)
{
	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_SUBMENU;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = subMenuName;
	subMenus[tempIndex][menuOptionIndex[tempIndex]] = subMenu;

	menuOptionIndex[tempIndex]++;
}

void MenuBase::createSubMenu(char *subMenuName, int subMenu)
{
	tabText[subMenu] = subMenuName;
	tabIndex++;
}

void MenuBase::addBool(char *optionName, bool *b)
{
	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_BOOL;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	menuBools[tempIndex][menuOptionIndex[tempIndex]] = b;

	menuOptionIndex[tempIndex]++;
}

void MenuBase::addOption(char *optionName, char *secondText)
{
	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	if (secondText != 0)
		optionSecondText[tempIndex][menuOptionIndex[tempIndex]] = secondText;

	menuOptionIndex[tempIndex]++;
}

void MenuBase::addList(char *optionName, int *intValue, char **list, int listSize)
{
	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_LIST;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	listInts[tempIndex][menuOptionIndex[tempIndex]] = intValue;
	lists[tempIndex][menuOptionIndex[tempIndex]] = list;
	listOptionCount[tempIndex][menuOptionIndex[tempIndex]] = listSize - 1;
	menuOptionIndex[tempIndex]++;
}

void MenuBase::addInt(char *optionName, int *intValue, int min, int max, int inc)
{
	if (*intValue < min)
		*intValue = min;
	else if (*intValue > max)
		*intValue = max;

	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_INT;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	listInts[tempIndex][menuOptionIndex[tempIndex]] = intValue;
	intMin[tempIndex][menuOptionIndex[tempIndex]] = min;
	intMax[tempIndex][menuOptionIndex[tempIndex]] = max;
	intInc[tempIndex][menuOptionIndex[tempIndex]] = inc;
	menuOptionIndex[tempIndex]++;
}

void MenuBase::addFloat(char *optionName, float *intValue, float min, float max, float inc, int decimalPlaces)
{
	if (*intValue < min)
		*intValue = min;
	else if (*intValue > max)
		*intValue = max;

	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_FLOAT;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	listFloats[tempIndex][menuOptionIndex[tempIndex]] = intValue;
	fMin[tempIndex][menuOptionIndex[tempIndex]] = min;
	fMax[tempIndex][menuOptionIndex[tempIndex]] = max;
	fInc[tempIndex][menuOptionIndex[tempIndex]] = inc;
	fDecPlace[tempIndex][menuOptionIndex[tempIndex]] = decimalPlaces;
	menuOptionIndex[tempIndex]++;
}

void MenuBase::addButton(char *optionName, void *function, int r3, int r4, int r5)
{
	int tempIndex = tabIndex - 1;
	optionType[tempIndex][menuOptionIndex[tempIndex]] = TYPE_OPTION_CALL;
	menuText[tempIndex][menuOptionIndex[tempIndex]] = optionName;
	funcs[tempIndex][menuOptionIndex[tempIndex]] = function;
	arg1[tempIndex][menuOptionIndex[tempIndex]] = r3;
	arg2[tempIndex][menuOptionIndex[tempIndex]] = r4;
	arg3[tempIndex][menuOptionIndex[tempIndex]] = r5;

	menuOptionIndex[tempIndex]++;
}

bool MenuBase::menuOpen()
{
	return vars->menuOpen;
}

void MenuBase::openMenu()
{
	vars->menuOpen = true;
}

void MenuBase::closeMenu()
{
	vars->menuOpen = false;
}

int keyDownTime = 0;
bool inSubMenu = false;

void MenuBase::buttonMonitoring()
{
	if (tools::keyIsDown(XINPUT_GAMEPAD_LEFT_THUMB) && tools::keyIsDown(XINPUT_GAMEPAD_X) && !menuButtons)
	{
		menuButtons = true;
		if(!vars->editSafeArea)
			vars->menuOpen = !vars->menuOpen;
	}
	else if (!tools::keyIsDown(XINPUT_GAMEPAD_LEFT_THUMB) && !tools::keyIsDown(XINPUT_GAMEPAD_X) && menuButtons)
		menuButtons = false;

	if (vars->menuOpen && (GetTickCount() - tickCount) > Wait)
	{
		if (tools::keyIsDown(XINPUT_GAMEPAD_RIGHT_SHOULDER) && !rightBumper)
		{
			rightBumper = true;
			if (currentMenu <= 4)
			{
				if (currentMenu < 4)
					currentMenu++;
				else currentMenu = 0;
			}
		}
		else if (!tools::keyIsDown(XINPUT_GAMEPAD_RIGHT_SHOULDER) && rightBumper)
			rightBumper = false;

		if (tools::keyIsDown(XINPUT_GAMEPAD_LEFT_SHOULDER) && !leftBumper)
		{
			leftBumper = true;
			if (currentMenu <= 4)
			{
				if (currentMenu > 0)
					currentMenu--;
				else currentMenu = 4;
			}
		}
		else if (!tools::keyIsDown(XINPUT_GAMEPAD_LEFT_SHOULDER) && leftBumper)
			leftBumper = false;

		if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Scroll[currentMenu] += 1;
			if (Scroll[currentMenu] > menuOptionIndex[currentMenu] - 1)
				Scroll[currentMenu] = 0;
			sleep(125);
		}

		if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_UP))
		{
			Scroll[currentMenu] -= 1;
			if (Scroll[currentMenu] < 0)
				Scroll[currentMenu] = menuOptionIndex[currentMenu] - 1;
			sleep(125);
		}

		switch (optionType[currentMenu][Scroll[currentMenu]])
		{
		case TYPE_OPTION_BOOL:
			if (tools::keyIsDown(XINPUT_GAMEPAD_A) && !selectButton)
			{
				selectButton = true;
				*menuBools[currentMenu][Scroll[currentMenu]] = !*menuBools[currentMenu][Scroll[currentMenu]];
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_A) && selectButton)
				selectButton = false;
			break;
		case TYPE_OPTION_LIST:
			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_LEFT) && !dpadLeft)
			{
				dpadLeft = true;
				if (*listInts[currentMenu][Scroll[currentMenu]] >= PLAYER_SUBMENU)
					*listInts[currentMenu][Scroll[currentMenu]] = 4;
				else if (*listInts[currentMenu][Scroll[currentMenu]] > 0)
					*listInts[currentMenu][Scroll[currentMenu]] -= 1;
				else if (*listInts[currentMenu][Scroll[currentMenu]] <= 0)
					*listInts[currentMenu][Scroll[currentMenu]] = listOptionCount[currentMenu][Scroll[currentMenu]];
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_DPAD_LEFT) && dpadLeft)
				dpadLeft = false;

			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_RIGHT) && !dpadRight)
			{
				dpadRight = true;
				if (*listInts[currentMenu][Scroll[currentMenu]] >= listOptionCount[currentMenu][Scroll[currentMenu]])
					*listInts[currentMenu][Scroll[currentMenu]] = 0;
				else
					*listInts[currentMenu][Scroll[currentMenu]] += 1;
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_DPAD_RIGHT) && dpadRight)
				dpadRight = false;
			break;
		case TYPE_OPTION_INT:
			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (*listInts[currentMenu][Scroll[currentMenu]] <= intMin[currentMenu][Scroll[currentMenu]])
					*listInts[currentMenu][Scroll[currentMenu]] = intMax[currentMenu][Scroll[currentMenu]];
				else
					*listInts[currentMenu][Scroll[currentMenu]] -= intInc[currentMenu][Scroll[currentMenu]];

				if (tools::keyIsDown(XINPUT_GAMEPAD_X))
					sleep(25);
				else
					sleep(75);
			}

			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (*listInts[currentMenu][Scroll[currentMenu]] >= intMax[currentMenu][Scroll[currentMenu]])
					*listInts[currentMenu][Scroll[currentMenu]] = intMin[currentMenu][Scroll[currentMenu]];
				else
					*listInts[currentMenu][Scroll[currentMenu]] += intInc[currentMenu][Scroll[currentMenu]];

				if (tools::keyIsDown(XINPUT_GAMEPAD_X))
					sleep(25);
				else
					sleep(75);
			}

			break;
		case TYPE_OPTION_FLOAT:
			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				*listFloats[currentMenu][Scroll[currentMenu]] -= fInc[currentMenu][Scroll[currentMenu]];
				if (*listFloats[currentMenu][Scroll[currentMenu]] < fMin[currentMenu][Scroll[currentMenu]])
					*listFloats[currentMenu][Scroll[currentMenu]] = fMax[currentMenu][Scroll[currentMenu]];

				if (tools::keyIsDown(XINPUT_GAMEPAD_X))
					sleep(25);
				else
					sleep(75);
			}

			if (tools::keyIsDown(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				*listFloats[currentMenu][Scroll[currentMenu]] += fInc[currentMenu][Scroll[currentMenu]];

				if (*listFloats[currentMenu][Scroll[currentMenu]] > fMax[currentMenu][Scroll[currentMenu]])
					*listFloats[currentMenu][Scroll[currentMenu]] = fMin[currentMenu][Scroll[currentMenu]];

				if (tools::keyIsDown(XINPUT_GAMEPAD_X))
					sleep(25);
				else
					sleep(75);
			}
			break;
		case TYPE_OPTION_CALL:
			if (tools::keyIsDown(XINPUT_GAMEPAD_A) && !selectButton)
			{
				selectButton = true;
				if (menu.arg1[menu.currentMenu][Scroll[currentMenu]] == -1 && menu.arg2[menu.currentMenu][Scroll[currentMenu]] == -1)
				{
					void(*function)() = (void(*)())menu.funcs[menu.currentMenu][Scroll[currentMenu]];
					function();
				}
				else if (menu.arg1[menu.currentMenu][Scroll[currentMenu]] != -1 && menu.arg2[menu.currentMenu][Scroll[currentMenu]] == -1)
				{
					void(*function)(int) = (void(*)(int))menu.funcs[menu.currentMenu][Scroll[currentMenu]];
					function(menu.arg1[menu.currentMenu][Scroll[currentMenu]]);
				}
				else if (menu.arg1[menu.currentMenu][Scroll[currentMenu]] != -1 && menu.arg2[menu.currentMenu][Scroll[currentMenu]] != -1 && menu.arg3[menu.currentMenu][Scroll[currentMenu]] == -1)
				{
					void(*function)(int, int) = (void(*)(int, int))menu.funcs[menu.currentMenu][Scroll[currentMenu]];
					function(menu.arg1[menu.currentMenu][Scroll[currentMenu]], menu.arg2[menu.currentMenu][Scroll[currentMenu]]);
				}
				else if (menu.arg1[menu.currentMenu][Scroll[currentMenu]] != -1 && menu.arg2[menu.currentMenu][Scroll[currentMenu]] != -1 && menu.arg3[menu.currentMenu][Scroll[currentMenu]] != -1)
				{
					void(*function)(int, int, int) = (void(*)(int, int, int))menu.funcs[menu.currentMenu][Scroll[currentMenu]];
					function(menu.arg1[menu.currentMenu][Scroll[currentMenu]], menu.arg2[menu.currentMenu][Scroll[currentMenu]], menu.arg3[menu.currentMenu][Scroll[currentMenu]]);
				}
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_A) && selectButton)
				selectButton = false;
			break;
		case TYPE_OPTION_SUBMENU:
			if (tools::keyIsDown(XINPUT_GAMEPAD_A) && !selectButton)
			{
				selectButton = true;
				returnSubMenu = currentMenu;
				currentMenu = subMenus[currentMenu][Scroll[currentMenu]];
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_A) && selectButton)
				selectButton = false;
			break;
		}

		if (currentMenu > 4 && tools::keyIsDown(XINPUT_GAMEPAD_B) && !backButton)
		{
			backButton = true;
			if (currentMenu == COLOR_SUBMENU)
				currentMenu = 3;
			else if (currentMenu == ESPCOLOR_SUBMENU)
				currentMenu = 2;
			else
				currentMenu = returnSubMenu;
			inSubMenu = false;
		}
		else if (!tools::keyIsDown(XINPUT_GAMEPAD_B) && backButton)
			backButton = false;

		if (menu.currentMenu == GAMERTAG_SUBMENU && menu.Scroll[menu.currentMenu] > 3 && menu.Scroll[menu.currentMenu] < 9)
		{
			if (tools::keyIsDown(XINPUT_GAMEPAD_A) && !selectButton)
			{
				selectButton = true;
				strcpy(vars->originalGamertag, (char*)menu.optionSecondText[menu.currentMenu][menu.Scroll[menu.currentMenu]]);
				func::setGamertag((char*)menu.optionSecondText[menu.currentMenu][menu.Scroll[menu.currentMenu]]);
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_A) && selectButton)
				selectButton = false;

			if (tools::keyIsDown(XINPUT_GAMEPAD_Y) && !xButton && !vars->enterNumCustomTag)
			{
				xButton = true;
				vars->enterNumCustomTag = true;
				func::setupEnterCustomTag();
			}
			else if (!tools::keyIsDown(XINPUT_GAMEPAD_Y) && xButton)
				xButton = false;
		}
	}

	if (vars->editSafeArea && (GetTickCount() - tickCount) > Wait)
	{
		vars->menuOpen = false;

		if (vars->safeArea.x > 0.0f && tools::keyIsDown(XINPUT_GAMEPAD_DPAD_LEFT))
			vars->safeArea.x -= 1.0f;
		else if (vars->safeArea.x < 30.0f && tools::keyIsDown(XINPUT_GAMEPAD_DPAD_RIGHT))
			vars->safeArea.x += 1.0f;
		else if (vars->safeArea.y > 0.0f && tools::keyIsDown(XINPUT_GAMEPAD_DPAD_UP))
			vars->safeArea.y -= 1.0f;
		else if (vars->safeArea.y < 30.0f && tools::keyIsDown(XINPUT_GAMEPAD_DPAD_DOWN))
			vars->safeArea.y += 1.0f;

		sleep(100);

		if (tools::keyIsDown(XINPUT_GAMEPAD_B))
		{
			vars->editSafeArea = false;
			vars->menuOpen = true;
		}

		if (tools::keyIsDown(XINPUT_GAMEPAD_X))
		{
			vars->safeArea = vector2();
		}
	}
}

char *MenuBase::returnFont()
{
#if defined(BO2)
	return "fonts/720/normalFont";
#else
	return "fonts/normalFont";
#endif
}

char *MenuBase::returnTitle()
{
#if defined(BO2)
	return "Aspire CE - BO2";
#elif defined (BO1)
	return "Aspire CE - BO1";
#elif defined (MW2)
	return "Aspire CE - MW2";
#elif defined (MW3)
	return "Aspire CE - MW3";
#endif
}

float MenuBase::returnFontSize()
{
#if defined(BO2)
	return 0.6f;
#else
	return 0.5f;
#endif
}