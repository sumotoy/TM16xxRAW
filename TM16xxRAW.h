


#ifndef TM16XXRAW_h
#define TM16XXRAW_h

#include "Arduino.h"

//#define DDDEBUG

#define TMCOM_WD     0x40 // write data to display command
#define TMCOM_RK     0x42 // Read key scanning data command
#define TMCOM_FA     0x44 // Fixed Address (0x40 auto increase)
#define TMDPULSE     0x80 // 1/16 pulse (bit3 0)
#define TMSTARTADRS  0xC0 // address 0xc0 -> 0xCF

#define TMRDTIME  		1 //1 microsecond

#if defined(__arm__) && defined(CORE_TEENSY)						//teensy3
	#define TEENSY3X
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY) //due
	#define ARDUE
#else																//arduino 8 bit
	#define ARDUX
#endif

class TM16xxRAW
{
 public:
 TM16xxRAW(const byte datap,const byte clockp,const byte strobep,byte maxr=8,byte maxc=8);
 virtual void begin(byte bright = 3,byte displayOn = 1);
 virtual void brightness(byte bright = 3);
/*  virtual byte getButtons(void); */
 virtual uint16_t getButtons(void);
 virtual void clearAll(void);
 virtual void setLed(byte col,byte row,byte val);
 virtual void setLed(byte led,byte val);
 virtual byte getColumn(byte col);
 virtual byte getLed(byte led);
 virtual byte decodeButton(uint16_t but);
 
 byte columsState[8];//all led state goes here
 
 protected:
	byte _data_pin;
	byte _clock_pin;
	byte _strobe_pin;
	byte _brightness;
	byte _displayOn;
	byte _maxRow;
	byte _maxCol;
	
	void	send(byte data);
	void	sendCommand(byte cmd);
	byte	receiveData(void);
	void 	sendData(byte address,byte data);
	void	digitalWriteSpecial(const byte pin,const byte val);
 private:
	byte	decodeLed(byte led);
#if defined(DDDEBUG)
	void   printByte(unsigned int data,byte len);
#endif
 };


#endif
