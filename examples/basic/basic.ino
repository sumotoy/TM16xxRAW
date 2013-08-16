#include <TM16xxRAW.h>

//data_pin,clock_pin,strobe_pin
TM16xxRAW tm(3,4,5);


void setup() {
//optional:brightness(0:7),displayOn(0=off,1=on)
 tm.begin();
//column,row,state(0,1)
 tm.setLed(0,1,1);
}

void loop() {
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