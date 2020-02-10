#pragma once

extern float white[4];
extern float black[4];
extern float red[4];
extern float green[4];
extern float blue[4];
extern float yellow[4];
extern float orange[4];
extern float purple[4];
extern float lightBlue[4];
extern float cyan[4];
extern float mint[4];
extern float friendColor[4];
extern float enemColor[4];
extern float hittableColor[4];
extern float visibleColor[4];
extern float ignoredColor[4];
extern float targetedClient[4];

extern float darkMenuColor[4];
extern float secondaryColor[4];
extern float lightMenuColor[4];
extern float xButtonColor[4];

extern float halfWhite[4];
extern float halfBlack[4];
extern float halfRed[4];
extern float halfGreen[4];
extern float halfBlue[4];
extern float halfYellow[4];
extern float halfOrange[4];
extern float halfPurple[4];
extern float halfLightBlue[4];
extern float halfCyan[4];
extern float halfMint[4];

class draw
{
public:
	static void rect(float x, float y, float w, float h, char *material, float *color, bool outline = 0, float *outlineColor = 0, float outlineWidth = 0);
	static void emptyRect(float x, float y, float w, float h, char *material, float *color, float outlineWidth);
	static void text(const char *text, float x, float y, char *font, float fontSize, float *color, char alignment, float rotation, int outline, bool background = false, float *backgroundColor = 0, float *outlineColor = 0, float outlineWidth = 0.0f);
	static void line(float x1, float y1, float x2, float y2, float *color, float width);
	static void corner(int x, int y, int w, int h, float *color);
	static void fps();
	static void hostName();
	static void hostGeoLocation();
	static void ping();
	static void mapName();
	static void crosshair();
	static void controls();
	static void visuals();
	static void checkBox(float x, float y, bool checked, float *checkColor);
	static void intSlider(float x, float y, int value, int min, int max);
	static void floatSlider(float x, float y, float value, float minI, float maxI, int decPlaces);
};