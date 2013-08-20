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
++++++++++++++++++++++++++++++++++
coded by Max MC Costa for s.u.m.o.t.o.y - sumotoy@gmail.com
note: if you want to use (even parts), inform to the author, thanks!
--------------------------------------------------------------------------------
TM1638
--------------------------------------------------------------------------------

*/

#ifndef TM16XXRAW_h
#define TM16XXRAW_h

#include "Arduino.h"

//#define DDDEBUG

const byte TMCOM_WD 	= 0x40;
const byte TMCOM_RK		= 0x42;
const byte TMCOM_FA		= 0x44;
const byte TMDPULSE		= 0x88;//0x80 off
const byte TMSTARTADRS	= 0xC0;
const byte TMRDTIME		= 1; 



#if defined(__arm__) && defined(CORE_TEENSY)						//teensy3
	#define TEENSY3X
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY) //due
	#define ARDUE
#else																//arduino 8 bit
	#define ARDUX
	#include "../digitalWriteFast/digitalWriteFast.h"
#endif

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))


class TM16xxRAW
{
 public:
 TM16xxRAW(const byte datap,const byte clockp,const byte strobep,byte maxr=8,byte maxc=8);
 virtual void begin(byte bright = 3);
 virtual void brightness(byte bright = 3);
 virtual uint32_t getButtons();
 virtual void clearAll(void);
 virtual void setLed(byte col,byte row,byte val,bool update);
 virtual void setLed(byte led,byte val,bool update);
 virtual byte getColumn(byte colNum);
 virtual void setColumn(byte colNum,byte colData);
 virtual void updateColumn(byte col=255);
 virtual byte getLed(byte led);

 
 protected:
	byte _data_pin;
	byte _clock_pin;
	byte _strobe_pin;
	byte _brightness;
	byte _maxRow;
	byte _maxCol;
	
	void		send(byte data);
	void		sendCommand(const byte cmd);
	void 		sendData(const byte address,byte data);
	void		digitalWriteSpecial(const byte pin,const byte val);
	byte 		columsState[8];//all led state goes here
	//uint32_t 	switchState;//all switch state goes here
 private:
	byte	detectColumn(byte led);
	void	sendLed(byte col,byte row,byte val,bool update);
#if defined(DDDEBUG)
	void   printByte(uint32_t data,byte len);
#endif 
 };
#endif
