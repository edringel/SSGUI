
#include "SSGui1.h"




int touchedObject;

void setup() {

  pinMode(RESET_LINE,OUTPUT);
  digitalWrite(RESET_LINE,HIGH);
  Serial.begin(9600);
 SPI.begin();
  SD.begin(BUILTIN_SDCARD);
    digitalWrite(RESET_LINE,LOW);
  delay(500);
    digitalWrite(RESET_LINE,HIGH);
    delay(500);
    SSG.begin(1);
    delay(1000);
  Serial.println("ILI9341 Test!");
  tft.setRotation(1);
  tft.setTextSize(3);
  delay(1000);
  ts.setRotation(1);
  delay(500);
  SSG.activateControl(1);

}

void loop() {

  // put your main code here, to run repeatedly:
touchedObject = SSG.manageTouch();
switch (touchedObject) {
  case 1:
  SSG.setObjFillColor(1,0xF800,true);
  delay(1000);
  SSG.setObjFillColor(1,0xFFFF,true);
  break;
  case 6: 
  SSG.drawScreen(2);
  break;
  case 7:
  SSG.drawScreen(1);
  break;
}


}
