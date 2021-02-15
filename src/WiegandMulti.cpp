#include "WiegandMulti.h"
#include "PinChangeInterrupt.h"

#if defined(ESP8266)
    #define INTERRUPT_ATTR ICACHE_RAM_ATTR
#elif defined(ESP32)
	#define INTERRUPT_ATTR IRAM_ATTR
#else
    #define INTERRUPT_ATTR
#endif

WIEGANDMULTI::WIEGANDMULTI()
{
}

unsigned long WIEGANDMULTI::getCode()
{
	return _code;
}

int WIEGANDMULTI::getWiegandType()
{
	return _wiegandType;
}

bool WIEGANDMULTI::available()
{
	bool ret;
	//noInterrupts();
	ret=DoWiegandConversion();
	//interrupts();
	return ret;
}

void WIEGANDMULTI::begin(void (*ISR_D0)(void), void (*ISR_D1)(void))
{
	begin(2,3, ISR_D0, ISR_D1);
}

void WIEGANDMULTI::begin(int pinD0, int pinD1, void (*ISR_D0)(void), void (*ISR_D1)(void))
{
	_lastWiegand = 0;
	_cardTempHigh = 0;
	_cardTemp = 0;
	_code = 0;
	_wiegandType = 0;
	_bitCount = 0;  
	pinMode(pinD0, INPUT);					// Set D0 pin as input
	pinMode(pinD1, INPUT);					// Set D1 pin as input
	
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pinD0), ISR_D0, FALLING);  // Hardware interrupt - high to low pulse
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pinD1), ISR_D1, FALLING);  // Hardware interrupt - high to low pulse
}

INTERRUPT_ATTR void WIEGANDMULTI::ReadD0 ()
{
	_bitCount++;				// Increament bit count for Interrupt connected to D0
	if (_bitCount>31)			// If bit count more than 31, process high bits
	{
		_cardTempHigh |= ((0x80000000 & _cardTemp)>>31);	//	shift value to high bits
		_cardTempHigh <<= 1;
		_cardTemp <<=1;
	}
	else
	{
		_cardTemp <<= 1;		// D0 represent binary 0, so just left shift card data
	}
	_lastWiegand = millis();	// Keep track of last wiegand bit received
}

INTERRUPT_ATTR void WIEGANDMULTI::ReadD1()
{
	_bitCount ++;				// Increment bit count for Interrupt connected to D1
	if (_bitCount>31)			// If bit count more than 31, process high bits
	{
		_cardTempHigh |= ((0x80000000 & _cardTemp)>>31);	// shift value to high bits
		_cardTempHigh <<= 1;
		_cardTemp |= 1;
		_cardTemp <<=1;
	}
	else
	{
		_cardTemp |= 1;			// D1 represent binary 1, so OR card data with 1 then
		_cardTemp <<= 1;		// left shift card data
	}
	_lastWiegand = millis();	// Keep track of last wiegand bit received
}

unsigned long WIEGANDMULTI::GetCardId (volatile unsigned long *codehigh, volatile unsigned long *codelow, char bitlength)
{
	if (bitlength==26)								// EM tag
		return (*codelow & 0x1FFFFFE) >>1;

	if (bitlength==24)
		return (*codelow & 0x7FFFFE) >>1;

	if (bitlength==34)								// Mifare 
	{
		*codehigh = *codehigh & 0x03;				// only need the 2 LSB of the codehigh
		*codehigh <<= 30;							// shift 2 LSB to MSB		
		*codelow >>=1;
		return *codehigh | *codelow;
	}

	if (bitlength==32) {
		return (*codelow & 0x7FFFFFFE ) >>1;
	}

	return *codelow;								// EM tag or Mifare without parity bits
}

char translateEnterEscapeKeyPress(char originalKeyPress) {
	switch(originalKeyPress) {
	case 0x0b:        // 11 or * key
		return 0x0d;  // 13 or ASCII ENTER

	case 0x0a:        // 10 or # key
		return 0x1b;  // 27 or ASCII ESCAPE

	default:
		return originalKeyPress;
	}
}

bool WIEGANDMULTI::DoWiegandConversion ()
{
	unsigned long cardID;
	unsigned long sysTick = millis();
	
	if ((sysTick - _lastWiegand) > 25)								// if no more signal coming through after 25ms
	{
		if ((_bitCount==24) || (_bitCount==26) || (_bitCount==32) || (_bitCount==34) || (_bitCount==8) || (_bitCount==4)) 	// bitCount for keypress=4 or 8, Wiegand 26=24 or 26, Wiegand 34=32 or 34
		{
			_cardTemp >>= 1;			// shift right 1 bit to get back the real value - interrupt done 1 left shift in advance
			if (_bitCount>32)			// bit count more than 32 bits, shift high bits right to make adjustment
				_cardTempHigh >>= 1;

			if (_bitCount==8)		// keypress wiegand with integrity
			{
				// 8-bit Wiegand keyboard data, high nibble is the "NOT" of low nibble
				// eg if key 1 pressed, data=E1 in binary 11100001 , high nibble=1110 , low nibble = 0001 
				char highNibble = (_cardTemp & 0xf0) >>4;
				char lowNibble = (_cardTemp & 0x0f);
				_wiegandType=_bitCount;					
				_bitCount=0;
				_cardTemp=0;
				_cardTempHigh=0;
				
				if (lowNibble == (~highNibble & 0x0f))		// check if low nibble matches the "NOT" of high nibble.
				{
					_code = (int)translateEnterEscapeKeyPress(lowNibble);
					return true;
				}
				else {
					_lastWiegand=sysTick;
					_bitCount=0;
					_cardTemp=0;
					_cardTempHigh=0;
					return false;
				}

				// TODO: Handle validation failure case!
			}
			else if (4 == _bitCount) {
				// 4-bit Wiegand codes have no data integrity check so we just
				// read the LOW nibble.
				_code = (int)translateEnterEscapeKeyPress(_cardTemp & 0x0000000F);

				_wiegandType = _bitCount;
				_bitCount = 0;
				_cardTemp = 0;
				_cardTempHigh = 0;

				return true;
			}
			else		// wiegand 26 or wiegand 34
			{
				cardID = GetCardId (&_cardTempHigh, &_cardTemp, _bitCount);
				_wiegandType=_bitCount;
				_bitCount=0;
				_cardTemp=0;
				_cardTempHigh=0;
				_code=cardID;
				return true;
			}
		}
		else
		{
			// well time over 25 ms and bitCount !=8 , !=26, !=34 , must be noise or nothing then.
			_lastWiegand=sysTick;
			_bitCount=0;			
			_cardTemp=0;
			_cardTempHigh=0;
			return false;
		}	
	}
	else
	return false;
}
