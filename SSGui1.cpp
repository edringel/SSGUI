#include "SSGui1.h"
#include "Int_decode1.h"
#include "inRect1.h"
#include "objToIndex.h"

// for use with touchscreen
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000



namespace std {
  void __throw_length_error(char const*) {
  }
}

ScrObj:: ScrObj (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC){
	UId = uid;
	ScreenID = screenid;
	ObjType = objtype;
	IsControl = (boolean) IC;
	BackColor = BC;
	ForeColor = FC;
	FillColor = FillC;
}


SSGuiScreen::SSGuiScreen(uint16_t FColor, uint16_t BColor){
	fc = FColor;
	bc = BColor;
	numobj = 0;
}


ScrCircle::ScrCircle (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString):ScrObj(uid,screenid,objtype,IC,FC,BC,FillC){

	x = decodeInt(&bString, ',');
	y = decodeInt(&bString, ',');
	r = bString.toInt();
	tlx = x - r;
	tly = y - r;
	brx = x + r;
	bry = y + r;
	th = 2*r;
	tw = 2*r;
}


ScrRect::ScrRect (int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString)
:ScrObj(uid,screenid,objtype,IC,FC,BC,FillC){

	x = decodeInt(&bString,',');
	y = decodeInt(&bString,',');
	h = decodeInt(&bString,',');
//last element needs special handling
w = bString.toInt();
	tlx = x;
	tly = y;
	brx = x + w;
	bry = y + h;
	th = h;
	tw = w;


}

ScrText::ScrText(int uid, int screenid, int objtype, int IC, uint16_t FC, uint16_t BC, uint16_t FillC, String bString)
:ScrObj(uid,screenid,objtype,IC,FC,BC,FillC){
	x = decodeInt(&bString,',');
	y = decodeInt(&bString,',');
	s = decodeInt(&bString,',');
	ScreenText = bString;
	char *temp=(char *)ScreenText.c_str();
	int16_t x1, y1; //dummies
	tft.setTextSize(s);
	tft.getTextBounds(temp,0 ,0 , &x1,&y1,&tw,&th);
	tlx = x;
	tly = y;
	brx = x + tw;
	bry = y + th;
}
	
void SSGuiScreen:: begin(int screen) {
	File configFile;
	String build = "";
	char nextchar;
	int tUid;
	int tScreenID;
	int tObjType;
	boolean tIsControl;
	uint16_t tBC;
	uint16_t tFC;
	uint16_t tFillC;
	activeScreen = screen;
	tft.begin();
	ts.begin();
	configFile = SD.open("rsrc.txt", FILE_WRITE);
	//reading position opened to the end of the file!!
	//read screen size into build
	configFile.seek(0);
	do {
		nextchar = configFile.read();
		if (nextchar == ',')
			break;
		build.concat(nextchar);
	} while (true);
	scrX = (int)build.toInt();
	build = "";
		do {
		nextchar = configFile.read();
		if (nextchar == '\n')
			break;
		build.concat(nextchar);
	} while (true);
	scrY = (int)build.toInt();
	build = "";
	do {
		nextchar = configFile.read();
		if (nextchar == '\n')
			break;
			build.concat(nextchar);
	}while (true);
	numobj = (int)build.toInt();
	SSGObj.resize(numobj);
//extract objects
for (int k = 0;k < numobj; k++){
build = "";	
	do{
		nextchar = configFile.read();
		if ((nextchar == '\n') || (!configFile.available())){
			if (!configFile.available())
				build.concat(nextchar);
			break;
		}
			build.concat(nextchar);
	}while (true);
	tUid = decodeInt(&build, ',');
	tScreenID = decodeInt(&build, ',');
	tObjType = decodeInt(&build,',');
	tIsControl = (boolean)decodeInt(&build,',');
	tFC = decodeInt(&build,',');
	tBC = decodeInt(&build,',');
	tFillC = decodeInt(&build,',');
switch (tObjType){
	case 1:
	SSGObj[k] = new ScrRect(tUid,tScreenID,tObjType, tIsControl, tFC,tBC,tFillC, build);
	break;
	case 2:
	SSGObj[k] = new ScrCircle(tUid, tScreenID, tObjType, tIsControl, tFC, tBC, tFillC, build);
	break;
	case 3: //text
	SSGObj[k] = new ScrText(tUid, tScreenID, tObjType, tIsControl, tFC, tBC, tFillC, build);
	break;
}
}
configFile.close();
// Serial.println("about to enter drawscreen");
tft.fillScreen(this->bc);
//  Serial.println("got past fill screen");
this->activeScreen = screen;
drawScreen(screen);
/*
	for (int i = 0; i < this->numobj; i++){
		Serial.print("Screen ID: "); Serial.print(SSGObj[i]->ScreenID); Serial.print("Object type: "); Serial.print(SSGObj[i]->ObjType); Serial.print("UID: "); Serial.println(SSGObj[i]->UId);
		if (SSGObj[i]->ScreenID == screen) {

			Serial.print("Drawing object: "); Serial.print(i); Serial.print(" "); Serial.print("UID "); Serial.println(SSGObj[i]->UId);
			this->drawObj(i);
		}
	}
	*/
}

void SSGuiScreen:: drawObj(int objnum) {
	/*
	when adding a new object, typecast as follows:
ScrRect *scr = (ScrRect *)SSGObj[objnum];
ScrCircle *scc = ((ScrCircle *)SSGObj[objnum]);
ScrText *sct = ((ScrText *)SSGObj[objnum]);
*/
int idxnum = objToIndex(objnum);
tft.setRotation(1);
	switch (SSGObj[idxnum]->ObjType){
		case 1:
		tft.drawRect(((ScrRect *)SSGObj[idxnum])->x,((ScrRect *)SSGObj[idxnum])->y,((ScrRect *)SSGObj[idxnum])->w,((ScrRect *)SSGObj[idxnum])->h,((ScrRect *)SSGObj[idxnum])->ForeColor);
		if (((ScrRect *)SSGObj[idxnum])->BackColor != ((ScrRect *)SSGObj[idxnum])->FillColor)
			tft.fillRect(((ScrRect *)SSGObj[idxnum])->x, ((ScrRect *)SSGObj[idxnum])->y, ((ScrRect *)SSGObj[idxnum])->w, ((ScrRect *)SSGObj[idxnum])->h, ((ScrRect *)SSGObj[idxnum])->FillColor);
		break;
		case 2:
		tft.drawCircle(((ScrCircle *)SSGObj[idxnum])->x, ((ScrCircle *)SSGObj[idxnum])->y, ((ScrCircle *)SSGObj[idxnum])->r, ((ScrCircle *)SSGObj[idxnum])->ForeColor);
		if (((ScrCircle *)SSGObj[idxnum])->BackColor != ((ScrCircle *)SSGObj[idxnum])->FillColor)
			tft.fillCircle(((ScrCircle *)SSGObj[idxnum])->x, ((ScrCircle *)SSGObj[idxnum])->y, ((ScrCircle *)SSGObj[idxnum])->r, ((ScrCircle *)SSGObj[idxnum])->FillColor);
		break;
		case 3: //text
		tft.setCursor(((ScrText *)SSGObj[idxnum])->x, ((ScrText *)SSGObj[idxnum])->y);
//		Serial.print(((ScrText *)SSGObj[objnum])->x); Serial.print("  "); Serial.println(((ScrText *)SSGObj[objnum])->y);
		tft.setTextSize(((ScrText *)SSGObj[idxnum])->s);
		tft.setTextColor(((ScrText *)SSGObj[idxnum])->ForeColor,((ScrText *)SSGObj[idxnum])->BackColor);
		tft.print(((ScrText *)SSGObj[idxnum])->ScreenText);
		break;
	}
}

void SSGuiScreen:: setObjForeColor(int objnum, uint16_t color, boolean reDraw){
	int idxnum = objToIndex(objnum);
	SSGObj[idxnum]->ForeColor = color;
	if (reDraw)
		drawObj(objnum);
}

void SSGuiScreen:: setObjBackColor(int objnum, uint16_t color, boolean reDraw){
	int idxnum = objToIndex(objnum);
		SSGObj[idxnum]->BackColor = color;
	if (reDraw)
		drawObj(objnum);
}

void SSGuiScreen:: setObjFillColor(int objnum, uint16_t color, boolean reDraw){
	int idxnum = objToIndex(objnum);
		SSGObj[idxnum]->FillColor = color;
	if (reDraw)
		drawObj(objnum);
}

boolean SSGuiScreen:: setObjText(int objnum, String newString, boolean reDraw){
	int idxnum = objToIndex(objnum);
	int Otype = SSGObj[idxnum]->ObjType;
	//may introduce dynamically named framed buttons, may want a different type
	if (Otype == 3) {
		((ScrText *)SSGObj[idxnum])->ScreenText = newString;
	char *temp=(char *)newString.c_str();
	int16_t x1, y1; //dummies
	uint16_t tw, th;
	tft.setTextSize(((ScrText *)SSGObj[idxnum])->s);
	tft.getTextBounds(temp,0 ,0 , &x1,&y1,&tw,&th);
//top left coordinates will not change
	SSGObj[idxnum]->brx = SSGObj[idxnum]->tlx + tw;
	SSGObj[idxnum]->bry = SSGObj[idxnum]->tly + th;
	SSGObj[idxnum]->tw = tw;
	SSGObj[idxnum]->th = th;
	if (reDraw)
		drawObj(objnum);
	return true;
	}
	else
		return false;
}

void SSGuiScreen:: eraseObj(int objnum) {
	int idxnum = objToIndex(objnum);
	uint16_t oldForeColor, oldBackColor, oldFillColor;
	oldForeColor = SSGObj[idxnum]->ForeColor;
	oldFillColor = SSGObj[idxnum]->FillColor;
	oldBackColor = SSGObj[idxnum]->BackColor;
	SSGObj[idxnum]->ForeColor = this->bc;
	SSGObj[idxnum]->BackColor = this->bc;
	SSGObj[idxnum]->FillColor = this->bc;
	drawObj(objnum);
	SSGObj[idxnum]->ForeColor = oldForeColor;
	SSGObj[idxnum]->BackColor = oldBackColor;
	SSGObj[idxnum]->FillColor = oldFillColor;
	
}

void SSGuiScreen::drawScreen(int whichScreen){
tft.fillScreen(this->bc);

this->activeScreen = whichScreen;
	for (int i = 0; i < this->numobj; i++){
		if (SSGObj[i]->ScreenID == whichScreen) {
			this->drawObj(SSGObj[i]->UId);
		}
	}
}

int SSGuiScreen::manageTouch() {
	uint16_t mapped_X;
	uint16_t mapped_Y;
	TS_Point p;
	boolean was_touched = false;
	was_touched = ts.touched();
	if (was_touched){
		p = ts.getPoint();
		mapped_X = map(p.x, TS_MINX, TS_MAXX, 10, 320);
		mapped_Y = map(p.y, TS_MINY, TS_MAXY, 10, 240);
//This is sloppy, inelegant, and brute force, but it will work for this purpose.
// will implement as a helper function so that better quality functionality can be swapped in
for (int i = 0; i < numobj; i++){
	if (SSGObj[i]->ScreenID == activeScreen){
		if (inRect(mapped_X, mapped_Y,SSGObj[i]->tlx, SSGObj[i]->tly, SSGObj[i]->brx, SSGObj[i]->bry)){
			return SSGObj[i]->UId;
		}
	} //screen is active	
} //for number of controls loop	
	}//if	
	return 0;
}

void SSGuiScreen:: activateControl(int objnum){
	int idxnum = objToIndex(objnum);
	SSGObj[idxnum]->IsControl = true;
}

void SSGuiScreen:: deactivateControl(int objnum){
	int idxnum = objToIndex(objnum);
	SSGObj[idxnum]->IsControl = false;
}

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
XPT2046_Touchscreen ts = XPT2046_Touchscreen(TOUCH_CS, 255);
SSGuiScreen SSG = SSGuiScreen(0xFFE0, 0x0000);