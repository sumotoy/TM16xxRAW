#include "TM16xxRAW.h"

TM16xxRAW::TM16xxRAW(const byte datap,const byte clockp,const byte strobep,byte maxr,byte maxc){
	this->_data_pin = datap;
	this->_clock_pin = clockp;
	this->_strobe_pin = strobep;
	if (maxr > 8) maxr = 8;
	if (maxc > 8) maxc = 8;
	this->_maxRow = maxr-1;
	this->_maxCol = maxc-1;
}


void TM16xxRAW::begin(byte bright,byte displayOn) {
	byte i;
	if (bright > 7){
		bright = 7;
	}
	if (displayOn > 1) displayOn = 1;
	this->_brightness = bright;
	this->_displayOn = displayOn;
	
	pinMode(this->_data_pin, OUTPUT);
	pinMode(this->_clock_pin, OUTPUT);
	pinMode(this->_strobe_pin, OUTPUT);
	digitalWrite(this->_strobe_pin, HIGH);//strobe should be hi
	digitalWrite(this->_clock_pin, HIGH);//clock should be hi
  
	sendCommand(TMCOM_WD);
	
	brightness(bright);
	
	digitalWrite(this->_strobe_pin, LOW);
	sendCommand(TMSTARTADRS);
	for (i=0;i<16;i++){
		sendCommand(0x00);
	}
	digitalWrite(this->_strobe_pin, HIGH);
	clearAll();
}

void TM16xxRAW::brightness(byte bright){
	if (bright > 7){
		bright = 7;
	}
	this->_brightness = bright;
	sendCommand(TMDPULSE | (_displayOn ? 8 : 0) | this->_brightness);
}

byte TM16xxRAW::getButtons(void) {
	byte keys = 0;
	byte i;
	digitalWrite(this->_strobe_pin, LOW);
	send(TMCOM_RK);
	for (i = 0; i < 4; i++) {
		keys |= receiveData() << i;
	}
	digitalWrite(this->_strobe_pin, HIGH);
	return keys;
}



void TM16xxRAW::clearAll(void) {
	byte i;
	for (i=0;i<8;i++){
		columsState[i] = 0b00000000;
		sendData(i*2,columsState[i]);
	}
}


void TM16xxRAW::setLed(byte col,byte row,byte val){
	if (col > this->_maxCol) col = this->_maxCol;
	if (row > this->_maxRow) row = this->_maxRow;	
	if (val == 0){//led off
		bitClear(columsState[col],row);
	} 
	else {
		bitSet(columsState[col],row);
	}
	sendData(col*2,columsState[col]);
}

byte TM16xxRAW::getColumn(byte col) {
	if (col > 7) col = 7;
	return columsState[col];
}

/* LOW LEVEL */

void TM16xxRAW::send(byte data) {
	byte i;
	for (i = 0; i < 8; i++) {
		digitalWrite(this->_clock_pin, LOW);
		digitalWrite(this->_data_pin, data & 1 ? HIGH : LOW);
		data >>= 1;
		digitalWrite(this->_clock_pin, HIGH);
	}	
}


void TM16xxRAW::sendCommand(byte cmd){
	digitalWrite(this->_strobe_pin, LOW);
	send(cmd);
	digitalWrite(this->_strobe_pin, HIGH);
}

byte TM16xxRAW::receiveData(void) {
	byte temp = 0;
	byte i;
	// Pull-up on
	pinMode(this->_data_pin, INPUT);
	digitalWrite(this->_data_pin, HIGH);
	// get data
	for (i = 0; i < 8; i++) {
		temp >>= 1;
		digitalWrite(this->_clock_pin, LOW);
		if (digitalRead(this->_data_pin)) temp |= TMDPULSE;
		digitalWrite(this->_clock_pin, HIGH);
	}
	// Pull-up off
	pinMode(this->_data_pin, OUTPUT);
	digitalWrite(this->_data_pin, LOW);
	return temp;
}


void TM16xxRAW::sendData(byte address, byte data) {
	sendCommand(TMCOM_FA);
	digitalWrite(this->_strobe_pin, LOW);
	send(TMSTARTADRS | address);
	send(data);
	digitalWrite(this->_strobe_pin, HIGH);
}