#include "TM16xxRAW.h"

#if defined(ARDUX)
	#include "../digitalWriteFast/digitalWriteFast.h"
#endif

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
	//set some pin
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
	if (bright > 7) bright = 7;
	this->_brightness = bright;
	sendCommand(TMDPULSE | (_displayOn ? 8 : 0) | this->_brightness);
}

byte TM16xxRAW::getButtons(void) {
	byte keys = 0;
	byte i;
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(TMCOM_RK);
	for (i = 0; i < 4; i++) {
		keys |= receiveData() << i;
	}
	digitalWriteSpecial(this->_strobe_pin, HIGH);
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


void TM16xxRAW::setLed(byte led,byte val){
	byte col = decodeLed(led);
	led = led - (8*col);
	if (val == 0){//led off
		bitClear(columsState[col],led);
	} else {
		bitSet(columsState[col],led);
	}
	sendData(col*2,columsState[col]);
}

byte TM16xxRAW::getLed(byte led){
	byte col = decodeLed(led);
	led = led - (8*col);
	return bitRead(columsState[col],led);
}


byte TM16xxRAW::getColumn(byte col) {
	if (col > 7) col = 7;
	return columsState[col];
}
/* PRIVATE */

byte TM16xxRAW::decodeLed(byte led) {
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

/* LOW LEVEL */
void TM16xxRAW::digitalWriteSpecial(const byte pin,const byte val) {
#if defined(ARDUE)
	digitalWrite(pin,val);
#else
	digitalWriteFast(pin,val);
#endif
}


void TM16xxRAW::send(byte data) {
	byte i;
	for (i = 0; i < 8; i++) {
		digitalWriteSpecial(this->_clock_pin, LOW);
		digitalWriteSpecial(this->_data_pin, data & 1 ? HIGH : LOW);
		data >>= 1;
		digitalWriteSpecial(this->_clock_pin, HIGH);
	}	
}


void TM16xxRAW::sendCommand(byte cmd){
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(cmd);
	digitalWriteSpecial(this->_strobe_pin, HIGH);
}

byte TM16xxRAW::receiveData(void) {
	byte temp = 0;
	byte i;
	// data pin will be input
#if defined(ARDUX)
	pinModeFast(this->_data_pin, INPUT);
#else
	pinMode(this->_data_pin, INPUT);
#endif
	digitalWriteSpecial(this->_data_pin, HIGH);
	// get data
	for (i = 0; i < 8; i++) {
		temp >>= 1;
		digitalWriteSpecial(this->_clock_pin, LOW);
#if defined(ARDUX)
		if (digitalReadFast(this->_data_pin)) temp |= TMDPULSE;
#else
		if (digitalRead(this->_data_pin)) temp |= TMDPULSE;
#endif
		digitalWriteSpecial(this->_clock_pin, HIGH);
	}
	// put back data pin as out
#if defined(ARDUX)
	pinModeFast(this->_data_pin, OUTPUT);
#else
	pinMode(this->_data_pin, OUTPUT);
#endif
	digitalWriteSpecial(this->_data_pin, LOW);
	return temp;
}


void TM16xxRAW::sendData(byte address, byte data) {
	sendCommand(TMCOM_FA);
	digitalWriteSpecial(this->_strobe_pin, LOW);
	send(TMSTARTADRS | address);
	send(data);
	digitalWriteSpecial(this->_strobe_pin, HIGH);
}
