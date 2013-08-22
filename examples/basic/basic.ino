/*
very basic demo to demostrate how easy drive a tm1638 with this library.
A tm1638 has a led matrix 8x8 (64 led) connected and 25 switches, only 3 pins
needed by arduino. The demo will detect switches and turn on/off corrispondent led
(from 0 to 24), the other leds are free to blink. You will notice that everithing works
pretty fast and with any disturb between key scan and leds even if share same pins!
I've noticed that apparently no debounce it's needed.
*/

#include <TM16xxRAW.h>

//Instantiate the library
//data_pin,clock_pin,strobe_pin
TM16xxRAW tm(3,4,5);


volatile byte but = 0;
volatile byte obut = 1;

//the following used just for graphic feedback example
unsigned long blinkTime = 0;
byte count = 24;

void setup() {
  // init chip
  //optional:brightness(0:7))
  tm.begin();
}



void loop() {
  //this is a simple visual feedback routine. It will drive leds from 24 to 63
  //in a kinda loop to dimostrate that keyscan is not so intrusive
  if (millis() -  blinkTime > 15){//we run out of time
    blinkTime = millis();//reset
    for (byte i=3;i<8;i++){//turn off leds of 6 remain column first
      tm.setColumn(i,0);
    }
    tm.setLed(count,1,false);//set a led with absolute addressing method
    tm.updateColumn();//update all colums!
    //fix the count var
    if (count > 63){
      count = 24;
    } 
    else {
      count++;
    }
  }
  //-------------------------------------------keyscan starts HERE
  uint32_t allButtons = tm.getButtons();// get button state
  if (allButtons != 0) {  // one or more button pressed
    if (obut != but){    // this prevent to trigger the same button at every loop cycle
    /* a simple routine that identify button pressed (and exit immediately)
    if you want detect more buttons at once you need another routine here
    */
      for (byte i=24; i>=0; i--){//getButtons give back a 32 bit var but only 24  used!!!     
        if (bitRead(allButtons,i) == 1){//found button pressed by checking bit
          but = i+1;
          obut = but;
          // this simple routine it's optional and only for visual feedback
          // it checks a led in led memory map to see if it's on or off and invert it's value
          if (tm.getLed(i)){
            tm.setLed(i,0,true);
          } 
          else {
            tm.setLed(i,1,true);
          }
          break;//as I said before, this will exit for loop
        }//end if
      }//end for
    }
  } 
  else { // no buttons pressed
    but = 0;  
  }
}


