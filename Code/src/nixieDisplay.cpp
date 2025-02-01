#include "nixieDisplay.h"

#include "Timer.h"
Timer T(MICROS);

//15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
//K0 K1 K2 K3 K4 K5 K6 K7 K8 K9 KP A1 A2 A3 A4 NC

nixieDisplay::nixieDisplay(int clockPin,
                          int dataPin, 
                          int samplePin)
{
    CK_PIN = clockPin;
    D_PIN = dataPin;
    S_PIN = samplePin;
    currentDigit=0;
	flag=false;
}

nixieDisplay::~nixieDisplay()
{
}


void nixieDisplay::nixieBegin(
                long refreshTime,
                int dutyCycle){

    pinMode(CK_PIN,OUTPUT);
    digitalWrite(CK_PIN,LOW); // per shiftout 
    pinMode(D_PIN,OUTPUT);
    pinMode(S_PIN,OUTPUT);

    T.setClock(refreshTime,dutyCycle);
    
    //clear register
    digitalWrite(S_PIN,LOW);
	shiftOut(D_PIN, CK_PIN, MSBFIRST, 0);
   	shiftOut(D_PIN, CK_PIN, MSBFIRST, 0);
	digitalWrite(S_PIN,HIGH);
}

void nixieDisplay::displayNixie(uint8_t a, bool ENa, uint8_t b , bool ENb,uint8_t dot){
    uint8_t Data[] = {(uint8_t)(a/10), 
                        (uint8_t)(a%10),
                        (uint8_t)(b/10),
                        (uint8_t)(b%10)};
    uint8_t EN[] ={ENa,ENa,ENb,ENb}; 

	uint16_t controlWord=0;

	if(T.getClock() && !flag){

		controlWord += 1<<(currentDigit+1);
		controlWord = EN[currentDigit] << ((9-Data[currentDigit])+6); 

		controlWord += 1<<(currentDigit+1);

        controlWord += dot<<(5);

		digitalWrite(S_PIN,LOW);
		shiftOut(D_PIN, CK_PIN, MSBFIRST, controlWord>>8);
   		shiftOut(D_PIN, CK_PIN, MSBFIRST, controlWord);
		digitalWrite(S_PIN,HIGH);

		currentDigit = (currentDigit+1) % 4;
		flag = true;
	}
	if(!T.getClock() && flag){
    	flag = false;
    	digitalWrite(S_PIN,LOW);
		shiftOut(D_PIN, CK_PIN, MSBFIRST, 0);
   		shiftOut(D_PIN, CK_PIN, MSBFIRST, 0);
		digitalWrite(S_PIN,HIGH);
  }
}