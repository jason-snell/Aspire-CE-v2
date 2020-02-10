#include "main.h"

float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
float orange[4] = { 1.0f, 0.63529f, 0.0f, 1.0f };
float purple[4] = { 0.65098f, 0.0f, 1.0f, 1.0f };
float lightBlue[4] = { 0.0f, 0.65098f, 1.0f, 1.0f };
float cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
float mint[4] = { 0.0f, 1.0f, 0.73333f, 1.0f };
float friendColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
float enemColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float hittableColor[4] = { 1.0f, 0.63529f, 0.0f, 1.0f };
float visibleColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
float ignoredColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float targetedClient[4] = { 1.0f, 0.0f, 1.0f, 1.0f };

float darkMenuColor[4] = { 0.0f, 0.470588f, 0.843137f, 1.0f };//{ 0.06274f, 0.06274f, 0.06274f, 1.0f };
float secondaryColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightMenuColor[4] = { 0.10588f, 0.10588f, 0.10588f, 1.0f };
float xButtonColor[4] = { 0.55686, 0.09411, 0.09411, 1.0f };

float halfWhite[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
float halfBlack[4] = { 0.0f, 0.0f, 0.0f, 0.5f };
float halfRed[4] = { 1.0f, 0.0f, 0.0f, 0.5f };
float halfGreen[4] = { 0.0f, 1.0f, 0.0f, 0.5f };
float halfBlue[4] = { 0.0f, 0.0f, 1.0f, 0.5f };
float halfYellow[4] = { 1.0f, 1.0f, 0.0f, 0.5f };
float halfOrange[4] = { 1.0f, 0.63529f, 0.0f, 0.5f };
float halfPurple[4] = { 0.65098f, 0.0f, 1.0f, 0.5f };
float halfLightBlue[4] = { 0.0f, 0.65098f, 1.0f, 0.5f };
float halfCyan[4] = { 0.0f, 1.0f, 1.0f, 0.5f };
float halfMint[4] = { 0.0f, 1.0f, 0.73333f, 0.5f };

void draw::rect(float x, float y, float w, float h, char *material, float *color, bool outline, float *outlineColor, float outlineWidth)
{
	if (outline)
	{
		R_AddCmdDrawStretchPic(x, y, outlineWidth, h, 0.0f, 0.0f, 1.0f, 1.0f, outlineColor, Material_RegisterHandle(material, 0)); // LEFT
		R_AddCmdDrawStretchPic(x + outlineWidth, y, w - outlineWidth, outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, outlineColor, Material_RegisterHandle(material, 0)); // TOP
		R_AddCmdDrawStretchPic(x + outlineWidth, y + h - outlineWidth, w - (outlineWidth * 2.0f), outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, outlineColor, Material_RegisterHandle(material, 0)); // BOTTOM
		R_AddCmdDrawStretchPic(x + w - outlineWidth, y + outlineWidth, outlineWidth, h - outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, outlineColor, Material_RegisterHandle(material, 0)); // RIGHT
	}
	R_AddCmdDrawStretchPic(x + outlineWidth, y + outlineWidth, w - (outlineWidth * 2.0f), h - (outlineWidth * 2.0f), 0, 0, 1, 1, color, Material_RegisterHandle(material, 0));
}

void draw::emptyRect(float x, float y, float w, float h, char *material, float *color, float outlineWidth)
{
	R_AddCmdDrawStretchPic(x, y, outlineWidth, h, 0.0f, 0.0f, 1.0f, 1.0f, color, Material_RegisterHandle(material, 0)); // LEFT
	R_AddCmdDrawStretchPic(x + outlineWidth, y, w - outlineWidth, outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, color, Material_RegisterHandle(material, 0)); // TOP
	R_AddCmdDrawStretchPic(x + outlineWidth, y + h - outlineWidth, w - (outlineWidth * 2.0f), outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, color, Material_RegisterHandle(material, 0)); // BOTTOM
	R_AddCmdDrawStretchPic(x + w - outlineWidth, y + outlineWidth, outlineWidth, h - outlineWidth, 0.0f, 0.0f, 1.0f, 1.0f, color, Material_RegisterHandle(material, 0)); // RIGHT
}

void draw::text(const char *text, float x, float y, char *font, float fontSize, float *color, char alignment, float rotation, int outline, bool background, float *backgroundColor, float *outlineColor, float outlineWidth)
{
	if (!text)
		return;

	void *font_ = R_RegisterFont(font, 0);
	float textWidth = (float)R_TextWidth(text, 0x7FFFFFFF, font_) * fontSize;
	float textHeight = (float)R_TextHeight(font_) * fontSize;

	switch (alignment)
	{
	default:
	case 'l':
	case 'L':
		if (background)
			rect(x, y, textWidth + (outlineWidth + 6.0f), textHeight, "white", backgroundColor, outlineColor != 0, outlineColor, outlineWidth);

		R_AddCmdDrawText(text, 0x7FFFFFFF, font_, x + (outlineWidth + 2.0f), y + textHeight, fontSize, fontSize, rotation, color, outline);
		break;
	case 'c':
	case 'C':
		if (background)
			rect(x - (textWidth / 2.0f) - (outlineWidth + 6.0f), y, (textWidth / 2.0f) + (outlineWidth + 6.0f), textHeight, "white", backgroundColor, outlineColor != 0, outlineColor, outlineWidth);

		R_AddCmdDrawText(text, 0x7FFFFFFF, font_, x - (textWidth / 2.0f), y + textHeight, fontSize, fontSize, rotation, color, outline);
		break;
	case 'r':
	case 'R':
		if (background)
			rect(x - textWidth - (outlineWidth + 6.0f), y, textWidth + (outlineWidth + 6.0f), textHeight, "white", backgroundColor, outlineColor != 0, outlineColor, outlineWidth);

		R_AddCmdDrawText(text, 0x7FFFFFFF, font_, x - textWidth - (outlineWidth + 3.0f), y + textHeight, fontSize, fontSize, rotation, color, outline);
		break;
	}
}

void draw::line(float x1, float y1, float x2, float y2, float *color, float width)
{
	float x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1) * 57.2957795f;

	CG_DrawRotatedPicPhysical(addr->screenplacement, x, y, l2, 1, angle, color, Material_RegisterHandle("white", 0));
}

void draw::corner(int x, int y, int w, int h, float *color)
{
	float lineW = (w / 5.0f);
	float lineH = (h / 6.0f);
	float lineT = 1.0f;

	line(x, y, x, y + lineH, color, 1.0f); // top left
	line(x, y, x + lineW + 1, y, color, 1.0f);
	line(x + w - lineW - 2, y, x + w, y, color, 1.0f); // top right
	line(x + w, y, x + w, y + lineH, color, 1.0f);
	line(x, y + h - lineH, x, y + h, color, 1.0f); // bottom left
	line(x - 1, y + h, x + lineW + 1, y + h, color, 1.0f);
	line(x + w - lineW - 2, y + h, x + w, y + h, color, 1.0f); // bottom right
	line(x + w, y + h - lineH, x + w, y + h, color, 1.0f);
}

float visualY = 5.0f;

void draw::fps()
{
	visualY += 20.0f;

	static int frameCount = 0;
	static float lastFps = 60.0f;
	frameCount++;

	float currentFps = *(float*)(addr->FPS);
	if (currentFps > 100.0f && currentFps < 200.0f)
		currentFps /= 2.0f;
	if (frameCount >= currentFps)
	{
		lastFps = currentFps;
		frameCount = 0;
	}
	char fps[32] = { 0 };
	sprintf(fps, "FPS: %.0f", lastFps);

	draw::text(fps, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
}

void draw::hostName()
{
	visualY += 20.0f;

	char totalBuffer[0xFF] = { 0 };
	int hostClientNum = func::hostClientNum();

	char host[40] = { 0 };
#if defined (MW2)
	strcpy(host, Dvar_GetString("sv_hostname"));
#elif defined (MW3)
	strcpy(host, (char*)(0x82720D90 + 0x15C));
#else
	strcpy(host, Dvar_GetString("party_hostname"));
#endif

	if (*(int*)host == 0 || hostClientNum == -1)
		strcpy(host, "None");

	char sIP[32] = { 0 };

	unsigned char IP[4] = { 0 };

	switch (vars->hostName)
	{
	case HOSTNAME_NAME:
		sprintf_s(totalBuffer, "Host: %s^7", host);
		break;
	case HOSTNAME_NAMEIP:
		memcpy(IP, (unsigned char*)(addr->netInfo + (hostClientNum * addr->netInfoSize) + addr->netInfoIP), 4);
		sprintf_s(sIP, "%i.%i.%i.%i", IP[0], IP[1], IP[2], IP[3]);
		if (hostClientNum != -1)
			sprintf_s(totalBuffer, "Host: %s^7 - IP: %s", host, sIP);
		else
			sprintf_s(totalBuffer, "Host: %s^7", host);
		break;
	case HOSTNAME_IP:
		memcpy(IP, (unsigned char*)(addr->netInfo + (hostClientNum * addr->netInfoSize) + addr->netInfoIP), 4);
		if (IP[0] != 0)
		{
			sprintf_s(sIP, "%i.%i.%i.%i", IP[0], IP[1], IP[2], IP[3]);
			sprintf_s(totalBuffer, "Host IP: %s^7", sIP);
		}
		else
		{
			sprintf_s(totalBuffer, "Host IP: None");
		}
		break;
	}

	draw::text(totalBuffer, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
}

void draw::hostGeoLocation()
{
	int hostClientNum = func::hostClientNum();
	static char host[64] = { 0 };
#if defined (MW2)
	strcpy(host, Dvar_GetString("sv_hostname"));
#elif defined (MW3)
	strcpy(host, (char*)(0x82720D90 + 0x15C));
#else
	strcpy(host, Dvar_GetString("party_hostname"));
#endif

	if (*(int*)host != 0 && hostClientNum != -1 && *(int*)State[hostClientNum] != 0 && !strstr(Country[hostClientNum], "Empty"))
	{
		visualY += 20.0f;
		static char country[100] = { 0 };
		sprintf_s(country, "Country: %s", Country[hostClientNum]);
		draw::text(country, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
		visualY += 20.0f;
		static char state[100] = { 0 };
		sprintf_s(state, "State: %s", State[hostClientNum]);
		draw::text(state, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
		visualY += 20.0f;
		static char city[100] = { 0 };
		sprintf_s(city, "City: %s", City[hostClientNum]);
		draw::text(city, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
		visualY += 20.0f;
		static char zip[32] = { 0 };
		sprintf_s(zip, "Zip Code: %s", ZipCode[hostClientNum]);
		draw::text(zip, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
		visualY += 20.0f;
		static char isp[100] = { 0 };
		sprintf_s(isp, "ISP: %s", ISP[hostClientNum]);
		draw::text(isp, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
	}
}

void draw::ping()
{
	visualY += 20.0f;
}

void draw::mapName()
{
	visualY += 20.0f;

	static char buffer[0xFF] = { 0 };
#if defined (BO2)
	auto UI_GetGameTypeOnMapName = (void(*)(char *outputString, const char *inputMapName, const char *inputGameType))addr->UI_GetGameTypeOnMapName_;
	UI_GetGameTypeOnMapName(buffer, Dvar_GetString("ui_mapname"), Dvar_GetString("ui_gametype"));
#elif defined (BO1)
	auto UI_GetGameTypeOnMapName = (void(*)(char *outputString, const char *inputMapName, const char *inputGameType))addr->UI_GetGameTypeOnMapName_;
	UI_GetGameTypeOnMapName(buffer, Dvar_GetString("ui_mapname"), Dvar_GetString("ui_gametype"));
#else
	char*(*UI_GetGameTypeDisplayName)(const char *dirtyName) = (char*(*)(const char *))addr->UI_GetGameTypeDisplayName_;
	char*(*UI_GetMapDisplayName)(const char *dirtyName) = (char*(*)(const char *))addr->UI_GetMapDisplayName_;
	char GameTypeDisplayName[64] = { 0 }, MapDisplayName[64] = { 0 };
	strcpy(GameTypeDisplayName, UI_GetGameTypeDisplayName(Dvar_GetString("ui_gametype")));
	strcpy(MapDisplayName, UI_GetMapDisplayName(Dvar_GetString("ui_mapname")));
	sprintf_s(buffer, "%s on %s", GameTypeDisplayName, MapDisplayName);
#endif
	draw::text(buffer, vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
}

void draw::crosshair()
{
	float centerX = vars->width / 2.0f;
	float centerY = vars->height / 2.0f;
	line(centerX - 10.0f, centerY - 10.0f, centerX + 10.0f, centerY + 10.0f, darkMenuColor, 1.0f);
	line(centerX + 10.0f, centerY - 10.0f, centerX - 10.0f, centerY + 10.0f, darkMenuColor, 10.0f);
}

void draw::controls()
{
	static char controlText[0x255] = { 0 };
	if (vars->menuOpen)
	{
		switch (menu.optionType[menu.currentMenu][menu.Scroll[menu.currentMenu]])
		{
		case TYPE_OPTION_BOOL:
#if defined(BO2)
			strncpy_s(controlText, "^BXENONButtonA^ To Select | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, " To Select |  +  To Close", 0x255);
#endif
			break;
		case TYPE_OPTION_LIST:
#if defined(BO2)
			strncpy_s(controlText, "^BXENONButtondpadL^/^BXENONButtondpadR^ To Switch | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, "/ To Switch |  +  To Close", 0x255);
#endif
			break;
		case TYPE_OPTION_INT:
#if defined(BO2)
			strncpy_s(controlText, "^BXENONButtondpadL^/^BXENONButtondpadR^ To Inc/Dec | ^BXENONButtonX^ To Speed Up | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, "/ To Inc/Dec |  To Speed Up |  +  To Close", 0x255);
#endif
			break;
		case TYPE_OPTION_FLOAT:
#if defined(BO2)
			strncpy_s(controlText, "^BXENONButtondpadL^/^BXENONButtondpadR^ To Inc/Dec | ^BXENONButtonX^ To Speed Up | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, "/ To Inc/Dec |  To Speed Up |  +  To Close", 0x255);
#endif
			break;
		case TYPE_OPTION_CALL:
#if defined(BO2)
			strncpy_s(controlText, "^BXENONButtonA^ To Select | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, " To Select |  +  To Close", 0x255);
#endif
			break;
		}

		if (menu.currentMenu == GAMERTAG_SUBMENU && menu.Scroll[menu.currentMenu] > 3 && menu.Scroll[menu.currentMenu] < 9)
		{
#if defined (BO2)
			strncpy_s(controlText, "^BXENONButtonA^ To Set Gamertag | ^BXENONButtonY^ To Edit Gamertag | ^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Close", 0x255);
#else
			strncpy_s(controlText, " To Set Gamertag |  To Edit Gamertag |  +  To Close", 0x255);
#endif
		}
	}
	else
	{
#if defined(BO2)
		strncpy_s(controlText, "^BXENONButtonStickAnimatedL^ + ^BXENONButtonX^ To Open/Close", 0x255);
#else
		strncpy_s(controlText, " + To Open", 0x255);
#endif
	}

	draw::text(controlText, vars->width - 5.0f - vars->safeArea.x, vars->height - 22.0f - vars->safeArea.y, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);
}

void draw::visuals()
{
	visualY = 5.0f + vars->safeArea.y;

	draw::text(menu.returnTitle(), vars->width - 5.0f - vars->safeArea.x, visualY, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, false, true, halfBlack, darkMenuColor, 1.0f);

	if (vars->fps)
		fps();

	if (vars->hostName != HOSTNAME_OFF)
		hostName();

	if (vars->hostGeoLocation)
		hostGeoLocation();

	if (vars->mapName)
		mapName();

	if (func::inGame() && vars->crosshair)
		crosshair();

	if (vars->controls)
		controls();

	if (vars->healthBar && func::inGame())
	{
		vector2 cord = vector2(vars->width / 2.0f, vars->height);
		cord.y = cord.y - 50.0f;
		cord.x = cord.x - 100.0f;

		if (cg->Health > 0)
		{
			float r = 0.0f, g = 0.0f, b = 0.0f, multiplier = 200.0f / (float)cg->maxHealth;
			r = 255.0f - ((float)cg->Health * multiplier);
			g = (float)cg->Health * multiplier;
			float healthWidth = 0.0f;
			if (cg->Health > 0)
			{
				healthWidth = cg->Health * (multiplier);
			}
			float healthColor[4] = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };

			float x = vars->width / 2.0f, y = 680.0f - vars->safeArea.y;

			draw::rect(x - 102.0f, y - 2, 204.0f, 20.0f, "white", halfBlack);
			draw::rect(x - 100.0f, y, healthWidth, 16.0f, "white", healthColor);
			char healthText[32] = { 0 };
			sprintf(healthText, "%i/%i", cg->Health, cg->maxHealth);
			draw::text(healthText, x, y - 2.0f, menu.returnFont(), menu.returnFontSize(), white, 'C', 0.0f, 6);
		}
	}

	if (vars->editSafeArea)
	{
		draw::corner(5.0f + vars->safeArea.x, 5.0f + vars->safeArea.y, vars->width - 10.0f - vars->safeArea.x * 2.0f, vars->height - 10.0f - vars->safeArea.y * 2.0f, darkMenuColor);
		draw::rect(5.0f + vars->safeArea.x + 2.0f, 5.0f + vars->safeArea.y + 2.0f, vars->width - 10.0f - vars->safeArea.x * 2.0f - 4.0f, vars->height - 10.0f - vars->safeArea.y * 2.0f - 4.0f, "white", halfBlack);
#if defined (BO2)
		draw::text("^BXENONButtondpadL^^BXENONButtondpadR^^BXENONButtondpadU^^BXENONButtondpadD^ To Change | ^BXENONButtonX^ To Reset | ^BXENONButtonB^ To Close", vars->width / 2.0f, vars->height / 2.0f, menu.returnFont(), 1.0f, secondaryColor, 'C', 0.0f, 6);
#else
		draw::text(" To Change |  To Reset |  To Close", vars->width / 2.0f, vars->height / 2.0f, menu.returnFont(), 1.0f, secondaryColor, 'C', 0.0f, 6);
#endif
	}
}

void draw::checkBox(float x, float y, bool checked, float *checkColor)
{
	rect(x, y, 15.0f, 15.0f, "white", lightMenuColor, true, secondaryColor, 1.0f);
	if (checked)
	{
		CG_DrawRotatedPicPhysical(addr->screenplacement, x + 5.0f, y + 7.0f, 8.0f, 1.0f, -60.0f, checkColor, Material_RegisterHandle("white", 0));
		CG_DrawRotatedPicPhysical(addr->screenplacement, x + 3.0f, y + 9.0f, 5.0f, 1.0f, 33.0f, checkColor, Material_RegisterHandle("white", 0));
	}
}

void draw::intSlider(float x, float y, int value, int minI, int maxI)
{
	rect(x, y + 4.0f, 100.0f, 2.0f, "white", secondaryColor);
	rect(x + ((100.0f / (maxI - minI)) * (value - minI) - 2.0f), y, 5.0f, 10.0f, "white", secondaryColor);

	char textBuff[32] = { 0 };
	sprintf_s(textBuff, "<%i/%i>", value, maxI);
	text(textBuff, x - 2.0f, y - 4.0f, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, 6);
}

void draw::floatSlider(float x, float y, float value, float minI, float maxI, int decPlaces)
{
	rect(x, y + 4.0f, 100.0f, 2.0f, "white", secondaryColor);
	rect(x + ((100.0f / (maxI - minI)) * (value - minI) - 2.0f), y, 5.0f, 10.0f, "white", secondaryColor);

	char textBuff[32] = { 0 };
	switch (decPlaces)
	{
	case 1: sprintf_s(textBuff, "<%.1f/%.1f>", fabsf(value), maxI);
		break;
	case 2: sprintf_s(textBuff, "<%.2f/%.2f>", fabsf(value), maxI);
		break;
	case 3: sprintf_s(textBuff, "<%.3f/%.3f>", fabsf(value), maxI);
		break;
	case 4: sprintf_s(textBuff, "<%.4f/%.4f>", fabsf(value), maxI);
		break;
	default: sprintf_s(textBuff, "<%.1f/%.1f>", fabsf(value), maxI);
		break;
	}
	text(textBuff, x - 2.0f, y - 4.0f, menu.returnFont(), menu.returnFontSize(), secondaryColor, 'R', 0.0f, 6);
}