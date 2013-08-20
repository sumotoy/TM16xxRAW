/*
very basic demo to demostrate how easy drive a tm1638 with this library.
A tm1638 has a led matrix 8x8 (64 led) connected and 25 switches, only 3 pins
needed by arduino. The demo will detect switches and turn on/off corrispondent led
(from 0 to 24), the other leds are free to blink. You will notice that everithing works
pretty fast and with any disturb between key scan and leds even if share same pins!
I've noticed that apparently no debounce it's needed.
*/

#include <TM16xxRAW.h>


//data_pin,clock_pin,strobe_pin
TM16xxRAW tm(3,4,5);


volatile byte but = 0;
volatile byte obut = 1;
unsigned long blinkTime = 0;

void setup() {
  //Serial.begin(38400);
  //optional:brightness(0:7))
  tm.begin();
}

byte count = 24;

void loop() {
  //randomLed2();
  if (millis() -  blinkTime > 15){
    blinkTime = millis();
    ledefx1(count);
    if (count > 63){
      count = 24;
    } 
    else {
      count++;
    }
  }

  uint32_t allButtons = tm.getButtons();
  if (allButtons != 0) {
    if (obut != but){
      for (byte i=24; i>=0; i--){
        if (bitRead(allButtons,i) == 1){
          but = i+1;
          obut = but;
          if (tm.getLed(i)){
            tm.setLed(i,0,true);
          } 
          else {
            tm.setLed(i,1,true);
          }
          //Serial.println(i+1,DEC);
          break;
        }
      }
    }
  } 
  else {
    but = 0;
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

void ledefx1(byte count){
  //  if (tm.getLed(count) == 1){
  //    tm.setLed(count,0,true);  
  //  } 
  //  else {
  //    tm.setLed(count,1,true);  
  //  }


  for (byte i=3;i<8;i++){
    tm.setColumn(i,0);
  }
  tm.setLed(count,1,false);
  tm.updateColumn();
}


