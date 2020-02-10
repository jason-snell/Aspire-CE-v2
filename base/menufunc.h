#pragma once

enum OPTION_TYPE : int
{
	TYPE_OPTION_CALL,
	TYPE_OPTION_BOOL,
	TYPE_OPTION_SUBMENU,
	TYPE_OPTION_FLOAT,
	TYPE_OPTION_INT,
	TYPE_OPTION_LIST,
	TYPE_OPTION,
};

class MenuBase
{
private:
public:
	bool bMenuOpen, menuButtons, dpadDown, dpadUp, selectButton, rightBumper, leftBumper, dpadLeft, dpadRight, backButton, xButton;
	const char *tabText[50];
	const char *menuText[50][50];
	const char *optionSecondText[50][50];
	bool *menuBools[50][50];
	int *listInts[50][50];
	float *listFloats[50][50];
	char **lists[50][50];
	int listOptionCount[50][50];
	int optionType[50][50];
	int menuOptionIndex[50];
	int returnSubMenu;
	int subMenus[50][50];
	int intMin[50][50];
	int intMax[50][50];
	int intInc[50][50];
	float fMin[50][50];
	float fMax[50][50];
	float fInc[50][50];
	int fDecPlace[50][50];
	void *funcs[50][50];
	int arg1[50][50];
	int arg2[50][50];
	int arg3[50][50];
	int newSubMenu[50][50];
	int tabIndex, maxScroll, currentMenu, Scroll[50];
	float menuPosX, menuPosY;

	MenuBase();
	~MenuBase();
	void addTab(char *text);
	void addSubMenu(char *subMenuName, int subMenu);
	void createSubMenu(char *subMenuName, int subMenu);
	void addOption(char *optionName, char *secondText = 0);
	void addBool(char *optionName, bool *b);
	void addList(char *optionName, int *intValue, char **list, int listSize);
	void addInt(char *optionName, int *intValue, int min, int max, int inc);
	void addFloat(char *optionName, float *intValue, float min, float max, float inc, int decimalPlaces);
	void addButton(char *optionName, void *function, int arg1 = -1, int arg2 = -1, int arg3 = -1);
	bool menuOpen();
	void openMenu();
	void closeMenu();
	void buttonMonitoring();
	char *returnFont();
	char *returnTitle();
	float returnFontSize();
};