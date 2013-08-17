#include <TM16xxRAW.h>


//data_pin,clock_pin,strobe_pin
TM16xxRAW tm(3,4,5);


volatile byte pButton = 0;
volatile byte pOldButton = 255;

/* -----------------------SETUP ------------------------*/
void setup() {
  Serial.begin(38400);
  //optional:brightness(0:7),displayOn(0=off,1=on)
  tm.begin();
  //column,row,state(0,1)
  tm.setLed(0,1,1);
}

/* ---------------------- LOOP -------------------------*/
void loop() {
  randomLed2();
  if (buttonFired()){
    Serial.println("Click!");
  } 
}


boolean buttonFired(){
  uint16_t but = tm.getButtons();
  if (but != 0) {
    pButton = tm.decodeButton(but);
    if (pButton != 0){
      if (pOldButton != pButton){
        pOldButton = pButton;
        Serial.println(pButton,DEC);
        return true;
      }
    } 
    else {
      return false;
    }
  }
  else {
    return false;
  }
}



//this use column,row led addressing
void randomLed1(){
  byte col = random(0,8);
  byte row = random(0,8);
  byte val = 0;
  if (bitRead(tm.getColumn(col),row) == 1){
    val = 0;
  } 
  else {
    val = 1;
  }
  tm.setLed(col,row,val);
  delay(5);
}

//this use just led addressing.
/*
using 8x8 matrix you have 64 led addressable.
 you can use any number from 0 to 63, the led are addressed as colums
 so led 0 to 7 will be in the column 0 and so on.
 */
void randomLed2(){
  byte led = random(0,63);
  byte val = 0;
  if (tm.getLed(led) == 1){
    val = 0;
  } 
  else {
    val = 1;
  }
  tm.setLed(led,val);  
  delay(5);
}



