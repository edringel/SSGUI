#ifndef SSGui_h
#define SSGui_h
#include "Arduino.h"
#include "stdio.h"
#include "stdarg.h"
#include <vector>
#include "SPI.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <XPT2046_Touchscreen.h>
#include "SD.h"

#define TFT_DC 26
#define TFT_CS 27
#define TOUCH_CS 10
#define TFT_MOSI 28
#define TFT_CLK 29
#define TFT_RST 255
#define TFT_MISO 30
#define RESET_LINE 25


class ScrObj {
	public:
 ScrObj (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC);
uint16_t tlx = 0;
uint16_t tly = 0;
uint16_t brx = 0;
uint16_t bry = 0;
uint16_t th = 0;  //for 4 bytes of storage lots fewer additions/subtractions
uint16_t tw = 0;
 int UId;
 int ScreenID;
 int ObjType;
 boolean IsControl;
 uint16_t BackColor;
 uint16_t ForeColor; 
 uint16_t FillColor;
};

class SSGuiScreen {
	public:
	SSGuiScreen(uint16_t FColor, uint16_t BColor);
	void begin(int screen);
	void drawObj(int objnum);
	void eraseObj(int objnum);
	boolean setObjText(int objnum, String newString, boolean reDraw);
	void setObjForeColor(int objnum, uint16_t color, boolean reDraw);
	void setObjBackColor(int objnum, uint16_t color, boolean reDraw);
	void setObjFillColor(int objnum, uint16_t color, boolean reDraw);
	void drawScreen(int whichScreen);
	void activateControl(int objnum);
	void deactivateControl(int objnum);
	int manageTouch();
	uint16_t fc;
	uint16_t bc;
	int numobj;
	int scrX;
	int scrY;
	int activeScreen = 0;
	std::vector<ScrObj*> SSGObj = {0};
};

class ScrRect : public ScrObj{
	public:
	ScrRect (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString);
	int16_t x;
	int16_t y;
	int16_t w;
	int16_t h;
};

class ScrCircle :public ScrObj{
	public:
	ScrCircle(int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString);
	int16_t x;
	int16_t y;
	int16_t r;
};

class ScrText : public ScrObj{
	/* theoretically, this could be a lot more complicated
	with embedded font, size, etc. for the moment, just include size.
	*/
	public:
	ScrText (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString);
	int16_t x;
	int16_t y;
	int16_t s;
	String ScreenText;
	};
	
extern	Adafruit_ILI9341 tft;
extern  XPT2046_Touchscreen ts;
extern  SSGuiScreen SSG;
	
#endif


