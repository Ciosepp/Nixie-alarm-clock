#include "Timer.h"

#include "nixieDisplay.h"

#define CLOCK_PIN 2
#define DATA_PIN 3
#define SAMPLE_PIN 4

nixieDisplay n(CLOCK_PIN,DATA_PIN,SAMPLE_PIN);

#define REFRESH_DUTYCYCLE 80
#define REFRESH_TIME_uS 2000


Timer T(MICROS);

Timer Ttime(MILLIS);

uint8_t digit=0;
bool busy=false;



void setup(){
	n.nixieBegin(REFRESH_TIME_uS,REFRESH_DUTYCYCLE);
 
}

void loop(){
  n.displayNixie(37, (millis()/1000)%60, 0);
  
}

