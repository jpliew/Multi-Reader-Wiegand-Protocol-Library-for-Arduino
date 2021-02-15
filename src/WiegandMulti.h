#ifndef _WIEGANDMULTI_H
#define _WIEGANDMULTI_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class WIEGANDMULTI {

private:
	bool DoWiegandConversion ();
	unsigned long GetCardId (volatile unsigned long *codehigh, volatile unsigned long *codelow, char bitlength);
	volatile unsigned long 	_cardTempHigh;
	volatile unsigned long 	_cardTemp;
	volatile unsigned long 	_lastWiegand;
	volatile int			_bitCount;	
	int						_wiegandType;
	unsigned long			_code;

public:
	WIEGANDMULTI();
	void begin(void (*ISR_D0)(void),void (*ISR_D1)(void));
	void begin(int pinD0, int pinD1, void (*ISR_D0)(void), void (*ISR_D1)(void));
	bool available();
	void ReadD0();
	void ReadD1();
	unsigned long getCode();
	int getWiegandType();
};

#endif
