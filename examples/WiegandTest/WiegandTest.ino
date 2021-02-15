#include <WiegandMulti.h>

WIEGANDMULTI wg;
WIEGANDMULTI wg2;

void Reader1D0Interrupt(void)
{
  wg.ReadD0();
}

void Reader1D1Interrupt(void)
{
  wg.ReadD1();
}

void Reader2D0Interrupt(void)
{
  wg2.ReadD0();
}

void Reader2D1Interrupt(void)
{
  wg2.ReadD1();
}


void setup() {
	Serial.begin(9600);  
	wg.begin(2,3,Reader1D0Interrupt,Reader1D1Interrupt);
	wg2.begin(4,5,Reader2D0Interrupt,Reader2D1Interrupt);
}

void loop() {
	if(wg.available())
	{
		Serial.print("Wiegand1 HEX = ");
		Serial.print(wg.getCode(),HEX);
		Serial.print(", DECIMAL = ");
		Serial.print(wg.getCode());
		Serial.print(", Type W");
		Serial.println(wg.getWiegandType());    
	}
	
	if(wg2.available())
	{
		Serial.print("Wiegand2 HEX = ");
		Serial.print(wg2.getCode(),HEX);
		Serial.print(", DECIMAL = ");
		Serial.print(wg2.getCode());
		Serial.print(", Type W");
		Serial.println(wg2.getWiegandType());    
	}
}