#include "Timer.h"

#define clockPin 2
#define dataPin 3
#define samplePin 4


#define REFRESH_DUTYCYCLE 80
#define REFRESH_TIME_uS 2000



Timer T(MICROS);

Timer Ttime(MILLIS);

void beginNIXIE(){
	T.setClock(REFRESH_TIME_uS,REFRESH_DUTYCYCLE);//
}
uint8_t digit=0;
bool busy=false;

uint16_t controlWord=0;
//15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
//K0 K1 K2 K3 K4 K5 K6 K7 K8 K9 KP A1 A2 A3 A4 NC


void displayNixie(uint8_t a, uint8_t b ){

	uint8_t Data[] = {(uint8_t)(a/10), (uint8_t)(a%10),(uint8_t)(b/10),(uint8_t)(b%10)};

	if(T.getClock() && !busy){

		controlWord = 1 << ((9-Data[digit])+6); 

		controlWord += 1<<(digit+1);

		digitalWrite(samplePin,LOW);
		shiftOut(dataPin, clockPin, MSBFIRST, controlWord>>8);
   		shiftOut(dataPin, clockPin, MSBFIRST, controlWord);
		digitalWrite(samplePin,HIGH);

		digit = (digit+1) % 4;
		busy = true;
	}
	if(!T.getClock() && busy){
    	busy = false;
    	digitalWrite(samplePin,LOW);
		shiftOut(dataPin, clockPin, MSBFIRST, 0);
   		shiftOut(dataPin, clockPin, MSBFIRST, 0);
		digitalWrite(samplePin,HIGH);
  }
}
void setup(){
	pinMode(clockPin,OUTPUT);
	pinMode(dataPin,OUTPUT);
	pinMode(samplePin,OUTPUT);
 	beginNIXIE();
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
}

void loop(){
  displayNixie(37, (millis()/1000)%60);
  
}

