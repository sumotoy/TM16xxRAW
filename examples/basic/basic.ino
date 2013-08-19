#include <TM16xxRAW.h>


//data_pin,clock_pin,strobe_pin
TM16xxRAW tm(3,4,5);


volatile uint32_t pbutton = 0;


void setup() {
  Serial.begin(38400);
  //optional:brightness(0:7))
  tm.begin();
}

void loop() {
  randomLed2();
  uint32_t pbutton = tm.getButtons();
  if (pbutton != 0) {
    Serial.println(pbutton,DEC);
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
  tm.setLed(col,row,val,true);
  delay(5);
}

//this use just led addressing.
/*
using 8x8 matrix you have 64 led addressable.
 you can use any number from 0 to 63, the led are addressed as colums
 so led 0 to 7 will be in the column 0 and so on.
 */
void randomLed2(){
  byte led = random(24,63);
  byte val = 0;
  if (tm.getLed(led) == 1){
    val = 0;
  } 
  else {
    val = 1;
  }
  tm.setLed(led,val,true);  
  delay(5);
}



