/*
                               _                
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 

--------------------------------------------------------------------------------
A universal library for drive TM1638 - TM1640 chip with any arduino or teensy
++++++++++++++++++++++++++++++++++
VERSION 0.3 (21 august 2013)
VERSION 0.5 (25 january 2014)
++++++++++++++++++++++++++++++++++
coded by Max MC Costa for s.u.m.o.t.o.y - sumotoy@gmail.com
note: if you want to use (even parts), inform to the author, thanks!
--------------------------------------------------------------------------------
TM1638
--------------------------------------------------------------------------------

*/

#include "TM16xxRAW.h"

/*
	instantiate library (mostly pins and some vars)
*/
TM16xxRAW::TM16xxRAW(const byte datap,const byte clockp,const byte strobep,byte maxr,byte maxc){
	this->_data_pin = datap;
	this->_clock_pin = clockp;
	this->_strobe_pin = strobep;
	if (maxr > 8) maxr = 8;
	if (maxc > 8) maxc = 8;
	this->_maxRow = maxr-1;
	this->_maxCol = maxc-1;
}

/*
	initiate chip and set it cleared
	Optionally you can set brightness (default = 3)
*/
void TM16xxRAW::begin(byte bright) {
	byte i;
	if (bright > 7) bright = 7;
	this->_brightness = bright;
	for (i=0;i<8;i++){//reset led memory map
		this->columsState[i] = 0b00000000;
	}
	//this->switchState = 0b000000000000000000000000;//reset switch map (only 24 bits used)
	//set some pin
	pinMode(this->_data_pin, OUTPUT);
	pinMode(this->_clock_pin, OUTPUT);
	pinMode(this->_strobe_pin, OUTPUT);
	digitalWriteSpecial(this->_strobe_pin, HIGH);//strobe should be hi
	digitalWriteSpecial(this->_clock_pin, HIGH);//clock should be hi
	// chip init
	sendCommand(TMCOM_WD);
	brightness(bright);
	digitalWriteSpecial(this->_strobe_pin, LOW);
	sendCommand(TMSTARTADRS);
	for (i=0;i<16;i++){
		sendCommand(0x00);
	}
	digitalWriteSpecial(this->_strobe_pin, HIGH);
	// end chip init
	clearAll(); // clear all leds
	
}

/*
	set the chip brightness (from 0 to 7)
*/
void TM16xxRAW::brightness(byte bright){
	if (bright > 7) bright = 7;
	this->_brightness = bright;
	sendCommand(TMDPULSE | this->_brightness);
}

/*
	clears internal ram and chip
*/
void TM16xxRAW::clearAll(void) {
	byte i;
	for (i=0;i<8;i++){
		this->columsState[i] = 0b00000000;
		sendData(i*2,this->columsState[i]);
	}
}


/*
	if no button pushed it's always 0
	gives back a 32bit var with the map of all switch pressed
*/
uint32_t TM16xxRAW::getButtons() {
	byte i;
	byte j;
	uint32_t out = 0;
	byte plus =0;
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(TMCOM_RK);
	for (i = 0;i < 4;i++) {
#if defined(ARDUX)
		pinModeFast(this->_data_pin,INPUT);// set data pin as input
#else
		pinMode(this->_data_pin,INPUT);
#endif
		digitalWriteSpecial(this->_data_pin,HIGH);// pullup data pin
		for (j = 0;j < 8;j++) { //loop to get 1 byte (create clock stream and read data)
			digitalWriteSpecial(this->_clock_pin,LOW);
			plus = j+(8*i);
			
			if (i == 0 && j < 4){
			} else if (i == 0 && j > 3){
				plus = plus - 1;
			} else if (i == 1 && j < 4){
				plus = plus - 2;
			} else if (i == 1 && j > 3){
				plus = plus - 3;
			} else if (i == 2 && j < 4){
				plus = plus - 4;
			} else if (i == 2 && j > 3){	
				plus = plus - 5;
			} else if (i == 3 && j < 4){	
				plus = plus - 6;
			} else {	
				plus = plus - 7;
			}
#if defined(ARDUX)
			if (digitalReadFast(this->_data_pin) == 1) bitSet(out,plus);
#else
			if (digitalRead(this->_data_pin) == 1) bitSet(out,plus);
#endif
			digitalWriteSpecial(this->_clock_pin,HIGH);	
		}
#if defined(ARDUX)
		pinModeFast(this->_data_pin,OUTPUT);// put back data pin as output
#else
		pinMode(this->_data_pin,OUTPUT);
#endif
		digitalWriteSpecial(this->_data_pin,LOW);// pull down data pin
	}
	digitalWriteSpecial(this->_strobe_pin, HIGH);
	return out;
}



/*
	set a single led on or off by using column and row address
*/
void TM16xxRAW::setLed(byte col,byte row,byte val,bool update){
	if (col <= this->_maxCol && row <= this->_maxRow) sendLed(col,row,val,update);
}

/*
	set a single led on or off by using led number
*/
void TM16xxRAW::setLed(byte led,byte val,bool update){
	if (led < ((this->_maxCol+1)*(this->_maxRow+1))){
		byte col = detectColumn(led);
		led = led - (8*col);
		sendLed(col,led,val,update);
	}
}


/*
	get the status of a single led in memory map
*/
byte TM16xxRAW::getLed(byte led){
	byte col = detectColumn(led);
	led = led - (8*col);
	return bitRead(this->columsState[col],led);
}

/*
	get the status of a single column (byte) in memory map
*/
byte TM16xxRAW::getColumn(byte colNum) {
	if (colNum > this->_maxCol) colNum = this->_maxCol;
	return this->columsState[colNum];
}

/*
	set an entire column in memory map
	*note:it doesn't update chip
*/
void TM16xxRAW::setColumn(byte colNum,byte colData) {
	if (colNum <= this->_maxCol) this->columsState[colNum] = colData;
}

/*
	this function send memory map to chip. It can send a single column
	or if set to 255 all columns in the same time
*/
void TM16xxRAW::updateColumn(byte col){
	if (col == 255){ //update all columns
		for (byte i=0;i<8;i++){
			sendData(i*2,this->columsState[i]);
		}
	} else if (col <= this->_maxCol){
		sendData(col*2,this->columsState[col]);
	}
}



/* PRIVATE */

/*
	this function detect the column by give the led
	note: has to be upgraded since allows only 8 col
*/
byte TM16xxRAW::detectColumn(byte led) {
	if (led < 8){						//col 0
		return 0;
	} else if (led > 7 && led < 16){	//col 1
		return 1;
	} else if (led > 15 && led < 24){	//col 2
		return 2;
	} else if (led > 23 && led < 32){	//col 3
		return 3;
	} else if (led > 31 && led < 40){	//col 4
		return 4;
	} else if (led > 39 && led < 48){	//col 5
		return 5;
	} else if (led > 47 && led < 56){	//col 6
		return 6;
	} else if (led > 55 && led < 64){	//col 7
		return 7;
	} else {							//col 7 - error
		return 7;
	}
}

/*
	update the state of a single led and optionally update the chip also
*/
void TM16xxRAW::sendLed(byte col,byte row,byte val,bool update){
	if (val == 0){//led off
		bitClear(this->columsState[col],row);
	} else {
		bitSet(this->columsState[col],row);
	}
	if (update) sendData(col*2,this->columsState[col]);
}

/* LOW LEVEL */

/*
	depends of cpu used this try to use the fastest way to write port
*/
void TM16xxRAW::digitalWriteSpecial(const byte pin,const byte val) {
#if defined(ARDUE)
	digitalWrite(pin,val);
#else
	digitalWriteFast(pin,val);
#endif
}

/*
	send a byte to chip
*/
void TM16xxRAW::send(byte data) {
	byte i;
	for (i = 0; i < 8; i++) {
		digitalWriteSpecial(this->_clock_pin, LOW);
		digitalWriteSpecial(this->_data_pin, data & 1 ? HIGH : LOW);
		data >>= 1;
		digitalWriteSpecial(this->_clock_pin, HIGH);
	}	
}

/*
	send a command to chip
*/
void TM16xxRAW::sendCommand(const byte cmd){
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(cmd);
	digitalWriteSpecial(this->_strobe_pin, HIGH);
}

/*
	send led data to chip
*/
void TM16xxRAW::sendData(const byte address, byte data) {
	sendCommand(TMCOM_FA);
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(TMSTARTADRS | address);
	send(data);
	digitalWriteSpecial(this->_strobe_pin, HIGH);
}



#if defined(DDDEBUG)
void TM16xxRAW::printByte(uint32_t data,byte len){
	if (data != 0){
		for (int i=(len-1); i>=0; i--){
			if (bitRead(data,i)==1){
				Serial.print("1");
			} 
			else {
				Serial.print("0");
			}
		}
		Serial.print(" -> 0x");
		Serial.print(data,HEX);
		Serial.print("\n");
	}
} 
#endif