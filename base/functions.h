#pragma once

class func
{
public:
	static float *callRainbow(float speed, int index);
	static double radians(float Degrees);
	static signed char clampChar(int i);
	static bool worldToScreen(vector3 &worldLocation, vector2 &Screen);
	static bool clientWorldToScreen(vector3 clientheadPos, vector3 clientViewAngles, vector3 &out);
	static bool getTagPos(int i, short tagName, vector3 &pos);
	static bool inGame();
	static bool notTeam(int i);
	static int playerState();
	static vector3 getViewOrigin();
	static int getWeaponShader(int weaponIndex);
	static void endGame();
	static int hostClientNum();
	static vector3 getClientViewAngles(int i);
	static void setClientShield(int client);
	static void setGamertag(char *gamertag);
	static void setupEnterCustomTag();
	static void downloadGeoLocation(int clientNum);
	static void toggleDLC();
};